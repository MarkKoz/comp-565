#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "io.h"

int compile_shader(const GLchar* const source, const GLuint* const shader)
{
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    return check_glsl_status(*shader, GL_COMPILE_STATUS);
}

int compile_shader_file(const char* const file_path, const GLuint* const shader)
{
    char* file_content = NULL;
    if (read_file(file_path, &file_content)) {
        return -1;
    }

    int result = compile_shader(file_content, shader);
    free(file_content);

    return result;
}

int load_shader(
    const char* const vertex_path, const char* const fragment_path, GLuint* const program_out)
{
    // Compile shaders.
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (compile_shader_file(vertex_path, &vertex_shader)) {
        return -1;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (compile_shader_file(fragment_path, &fragment_shader)) {
        return -1;
    }

    // Link shaders.
    // A shader program object is the linked version of multiple shaders combined.
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    if (check_glsl_status(program, GL_LINK_STATUS)) {
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    *program_out = program;

    return 0;
}

int check_glsl_status(const GLuint object, const GLenum status_type)
{
    int is_shader = status_type == GL_COMPILE_STATUS;
    int is_program = status_type == GL_LINK_STATUS || status_type == GL_VALIDATE_STATUS;

    GLint success = 0;
    if (is_shader) {
        glGetShaderiv(object, status_type, &success);
    } else if (is_program) {
        glGetProgramiv(object, status_type, &success);
    } else {
        fputs("ERROR: unsupported status_type type for check_glsl_status.\n", stderr);
        return -1;
    }

    if (success) {
        return 0;
    }

    GLint info_log_length = 0;
    if (is_shader) {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &info_log_length);
    } else {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &info_log_length);
    }

    char* info_log = malloc((size_t) info_log_length);
    if (info_log == NULL) {
        fputs("ERROR: Shader error; couldn't allocate memory for log buffer.\n", stderr);

        return -1;
    }

    if (is_shader) {
        glGetShaderInfoLog(object, info_log_length, NULL, info_log);
        fputs("ERROR: Failed to compile shader.\n", stderr);
    } else {
        glGetProgramInfoLog(object, info_log_length, NULL, info_log);
        fputs("ERROR: Failed to link or validate shader program.\n", stderr);
    }

    fputs(info_log, stderr);
    free(info_log);

    return -1;
}

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

    // Check for compilation errors.
    GLint success = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

    if (success) {
        return 0;
    }

    GLint info_log_length = 0;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &info_log_length);

    char* info_log = malloc((size_t) info_log_length);
    if (info_log == NULL) {
        fputs(
            "ERROR: Shader compilation failed; "
            "couldn't allocate memory for log buffer.\n",
            stderr);

        return -1;
    }

    glGetShaderInfoLog(*shader, info_log_length, NULL, info_log);
    fputs("ERROR: Shader compilation failed.\n", stderr);
    fputs(info_log, stderr);
    free(info_log);

    return -1;
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

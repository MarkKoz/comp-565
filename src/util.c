#include "util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

int load_shader(const GLchar* const source, const GLuint* const shader)
{
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    // Check for compilation errors
    int success = 0;
    char info_log[512];

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*shader, 512, NULL, info_log);
        fputs("ERROR: Shader compilation failed.\n", stderr);
        fputs(info_log, stderr);
        return -1;
    }

    return 0;
}

int load_shader_file(const char* const file_path, const GLuint* const shader)
{
    char* file_content = NULL;
    if (read_file(file_path, &file_content)) {
        return -1;
    }

    int result = load_shader(file_content, shader);
    free(file_content);

    return result;
}

int read_file(const char* const file_path, char** const output)
{
    if (file_path == NULL) {
        fputs("ERROR: Attempted to open a NULL file path.\n", stderr);
        return -1;
    }

    FILE* f = fopen(file_path, "r");
    if (f == NULL || errno) {
        perror("ERROR: Failed to open file");
        return -1;
    }

    // Seek to determine the file's length.
    // Only works for file lengths <= LONG_MAX.
    // Not guaranteed to work with binary streams.
    if (fseek(f, 0, SEEK_END)) {
        fputs("ERROR: Failed to seek to end of file.\n", stderr);
        return -1;
    }

    size_t length = (size_t) ftell(f);
    if (errno) {
        perror("ERROR: Failed to get file position");
        return -1;
    }

    rewind(f);

    // Allocate one extra byte for the null terminator.
    char* buffer = malloc(length + 1);
    if (buffer == NULL) {
        fputs("ERROR: Failed to allocate memory for file buffer.\n", stderr);
        return -1;
    }

    // EOF error is okay since there's one extra byte for termination anyway.
    fread(buffer, 1, length, f);
    if (ferror(f)) {
        fputs("ERROR: Failed to read file into buffer.\n", stderr);
        return -1;
    }

    // Null-terminate the string.
    buffer[length] = '\0';

    if (fclose(f)) {
        fputs("ERROR: Failed to close file.\n", stderr);
        return -1;
    }

    *output = buffer;
    return 0;
}

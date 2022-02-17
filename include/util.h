#ifndef CS_565_01_UTIL_H
#define CS_565_01_UTIL_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

int compile_shader(const GLchar* source, const GLuint* shader);

int compile_shader_file(const char* file_path, const GLuint* shader);

int read_file(const char* file_path, char** output);

#endif

#ifndef CS_565_01_UTIL_H
#define CS_565_01_UTIL_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

int load_shader(const GLchar* source, const GLuint* shader);

int load_shader_file(const char* file_path, const GLuint* shader);

int read_file(const char* file_path, char** output);

#endif

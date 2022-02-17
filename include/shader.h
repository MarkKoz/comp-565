#ifndef CS_565_01_SHADER_H
#define CS_565_01_SHADER_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

int compile_shader(const GLchar* source, const GLuint* shader);

int compile_shader_file(const char* file_path, const GLuint* shader);

#endif

#ifndef CS_565_01_SHADER_H
#define CS_565_01_SHADER_H

#include <glad/glad.h>

int compile_shader(const GLchar* source, const GLuint* shader);

int compile_shader_file(const char* file_path, const GLuint* shader);

int load_shader(const char* vertex_path, const char* fragment_path, GLuint* program_out);

int check_glsl_status(GLuint object, GLenum status_type);

#endif

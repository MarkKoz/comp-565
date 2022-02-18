#ifndef CS_565_01_VERTEX_H
#define CS_565_01_VERTEX_H

#include "glad/glad.h"

void init_tri_vertex_attr(GLuint* vao, GLuint* vbo, const float vertices[], GLsizeiptr size);

void update_vertex_colour(GLintptr vertex_index, float red, float green, float blue);

#endif

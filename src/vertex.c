#include "vertex.h"

#include <glad/glad.h>

void init_tri_vertex_attr(
    GLuint* const vao, GLuint* const vbo, const float vertices[], const GLsizeiptr size)
{
    // Create the vertex array object (VAO).
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    // Create the vertex buffer object (VBO).
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    // Copy the vertex data to the VBO.
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);

    // Position attribute:
    // The attribute's location is 0.
    // The amount of values is 3.
    // The type of each value as a float.
    // Disable fixed-point data type normalisation.
    // There are 2 sets of attributes per vertex, each of length 3. Hence, the stride is 6.
    // The starting offset for the attribute's data in the array is 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Colour attribute:
    // The attribute's location is 1.
    // The starting offset is 3 (skip the position attribute's values).
    // NOLINTNEXTLINE(performance-no-int-to-ptr)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO so other VAO calls won't accidentally modify this one.
    // This isn't strictly necessary in this case.
    glBindVertexArray(0);
}

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "shader.h"
#include "vertex.h"
#include "window.h"

// First 3 are positions; last 3 are colours.
// clang-format off
const float VERTICES[] = {
    0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F, // Bottom right
    -0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F, // Bottom left
    0.0F,  0.5F, 0.0F, 0.0F, 0.0F, 1.0F  // Top
};
// clang-format on

int main()
{
    GLFWwindow* window = NULL;
    if (init_window(1280, 720, "COMP 565 - Assignment 1", &window)) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Load all OpenGL function pointers.
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fputs("Failed to initialize GLAD.\n", stderr);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Load the shader.
    GLuint program = 0;
    if (load_shader("../resources/vertex.glsl", "../resources/fragment.glsl", &program)) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    GLuint vao = 0;
    GLuint vbo = 0;
    init_tri_vertex_attr(&vao, &vbo, VERTICES, sizeof(VERTICES));

    glUseProgram(program);

    // Render loop.
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // Render the triangle.
        // Bind the VAO so the set-up vertex attributes get used.
        glBindVertexArray(vao);
        // Draw a triangle using 3 vertices, starting at the 1st vertex.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return EXIT_SUCCESS;
}

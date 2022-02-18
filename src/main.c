#include <math.h>
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
    glBindVertexArray(vao); // Bind the VAO so the set-up vertex attributes get used.

    // Render loop.
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // Set the background colour.
        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // Alternate between 0 and 1 for a single colour component of each vertex.
        float colour = (float) (sin(glfwGetTime()) / 2.0 + 0.5);
        update_vertex_colour(0, colour, 0, 0);
        update_vertex_colour(1, 0, colour, 0);
        update_vertex_colour(2, 0, 0, colour);

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

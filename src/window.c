#include "window.h"

#include <stdio.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

int init_window(
    const int width, const int height, const char* const title, GLFWwindow** const window_out)
{
    // Initialise and configure GLFW.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the window.
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        fputs("Failed to create GLFW window.\n", stderr);
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    *window_out = window;

    return 0;
}

// NOLINTNEXTLINE(clang-diagnostic-unused-parameter, misc-unused-parameters)
void framebuffer_size_callback(GLFWwindow* const window, const int width, const int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

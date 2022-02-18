#ifndef CS_565_01_WINDOW_H
#define CS_565_01_WINDOW_H

#include <GLFW/glfw3.h>

int init_window(int width, int height, const char* title, GLFWwindow** window_out);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

#endif

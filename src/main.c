#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// settings
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "COMP 565 - LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        fputs("Failed to create GLFW window\n", stderr);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fputs("Failed to initialize GLAD\n", stderr);
        return -1;
    }

    // Build and compile the shaders.
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (compile_shader_file("../resources/vertex.glsl", &vertex_shader)) {
        return EXIT_FAILURE;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (compile_shader_file("../resources/fragment.glsl", &fragment_shader)) {
        return EXIT_FAILURE;
    }

    int success = 0;
    char info_log[512];

    // link shaders
    // A shader program object is the final linked version of multiple shaders combined.
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fputs("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", stderr);
        fputs(info_log, stderr);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // First 3 are positions; last 3 are colours.
    // ------------------------------------------------------------------
    // clang-format off
    float vertices[] = {
    0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F, // bottom right
    -0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F, // bottom left
    0.0F,  0.5F, 0.0F, 0.0F, 0.0F, 1.0F  // top
    };
    // clang-format on

    // Vertex Buffer Objects and Vertex Array Object
    unsigned int vbo = 0;
    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
    // vertex attributes(s).
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    // 1) The first parameter specifies which vertex attribute we want to configure.
    // Remember that we specified the location of the position vertex attribute in the vertex shader
    // with layout (location = 0). This sets the location of the vertex attribute to 0 and since we
    // want to pass data to this vertex attribute, we pass in 0. 2) The next argument specifies the
    // size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
    // 3) The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL
    // consists of floating point values). 4) The next argument specifies if we want the data to be
    // normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE,
    // the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float.
    // This is not relevant for us so we'll leave this at GL_FALSE.
    // 5) The fifth argument is known as the stride and tells us the space between consecutive
    // vertex attribute sets. Since the next set of position data is located exactly 3 times the
    // size of a float away we specify that value as the stride. Note that since we know that the
    // array is tightly packed (there is no space between the next vertex attribute value) we
    // could've also specified the stride as 0 to let OpenGL determine the stride (this only works
    // when values are tightly packed). 6) The last parameter is of type void* and thus requires
    // that weird cast. This is the offset of where the position data begins in the buffer. Since
    // the position data is at the start of the data array this value is just 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // color attribute
    // NOLINTNEXTLINE(performance-no-int-to-ptr)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but
    // this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we
    // want to
    glUseProgram(shader_program);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        process_input(window);

        // render the triangle
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react
// accordingly
// -------------------------------------------------------------------------------------------------
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
// NOLINTNEXTLINE(clang-diagnostic-unused-parameter, misc-unused-parameters)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

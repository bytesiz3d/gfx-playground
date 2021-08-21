#include <iostream>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Function prototypes
GLFWwindow* InitWindow(const GLuint width, const GLuint height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void WindowSizeCallback(GLFWwindow* window, int width, int height);

// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;

#include "shader.hpp"
#include "BasicTriangle.hpp"

int main()
{
    return Shader_Utils::TestQuad();

    // Create the window
    GLFWwindow* window = InitWindow(800, 600);
    if (window == NULL) return -1; 

    // Load and compile a shader
    GLuint shaderProgram = Shader::LoadShader("../shaders/color.vert", "../shaders/color.frag");
    glUseProgram(shaderProgram);

    BasicTriangle::Upload();

    // Main loop
    glClearColor(10.0f/256, 9.0f/256, 19.0f/256, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.)
        // and call corresponding response functions
        glfwPollEvents();

        // Clear the colorbuffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle
        BasicTriangle::Draw();

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    BasicTriangle::Destroy();
    return 0;
}

// Initializes the GLFW window and returns a pointer to it
GLFWwindow* InitWindow(GLuint width, GLuint height) 
{
    std::cout << "Starting GLFW context, OpenGL v4.0" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    WIDTH = width;
    HEIGHT = height;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GFX Playground", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return NULL;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    return window;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        std::cout << key << std::endl;
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Is called whenever the window is resized
void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    // Reset viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
}


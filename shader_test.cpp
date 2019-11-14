#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.cpp"
#include "shader.cpp"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void UploadData(GLuint shaderProgram);

int
main()
{
    const GLuint WIDTH = 1600, HEIGHT = 900;
    int MVP_location, tint_location;

    glfwInit();
    // Set all the required options for GLFW:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    GLuint shaderProgram = Shader::LoadShader("shader/color.vert", "shader/color.frag");
    glUseProgram(shaderProgram);

    // Get the location of the uniform variables:
    MVP_location = glGetUniformLocation(shaderProgram, "MVP");
    tint_location = glGetUniformLocation(shaderProgram, "tint");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    UploadData(shaderProgram);

    glBindVertexArray(0);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Black background

    Camera camera;
    camera.position = glm::vec3(0, 2, -3);
    camera.SetTarget(glm::vec3(0, 0, 0));
    camera.aspectRatio = WIDTH / HEIGHT;

    glm::mat4 MVP = camera.ViewProjectionMatrix();
    glUniformMatrix4fv(MVP_location, 1, false, glm::value_ptr(MVP));

    glm::mat4 tint = glm::mat4(1.f);
    glUniformMatrix4fv(tint_location, 1, false, glm::value_ptr(tint));

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void
UploadData(GLuint shaderProgram)
{
    float vertices[] = {
         -1,  1, -1,
         -1,  1,  1,
          1,  1,  1,
          1,  1, -1,
         //Lower Face
         -1, -1, -1,
          1, -1, -1,
          1, -1,  1,
         -1, -1,  1,
         //Right Face
          1, -1, -1,
          1,  1, -1,
          1,  1,  1,
          1, -1,  1,
          //Left Face
          -1, -1, -1,
          -1, -1,  1,
          -1,  1,  1,
          -1,  1, -1,
          //Front Face
          -1, -1,  1,
           1, -1,  1,
           1,  1,  1,
          -1,  1,  1,
          //Back Face
          -1, -1, -1,
          -1,  1, -1,
           1,  1, -1,
           1, -1, -1
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4 * 6, vertices, GL_STATIC_DRAW);

    GLuint position_attribute = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
    glEnableVertexAttribArray(position_attribute);

    GLubyte colors[] =
    {
        255, 255, 255, 255,
        255, 255, 255, 255,
        255, 255, 255, 255,
        255, 255, 255, 255,

        255, 0, 0, 255,
        255, 0, 0, 255,
        255, 0, 0, 255,
        255, 0, 0, 255,

        0, 0, 255, 255,
        0, 0, 255, 255,
        0, 0, 255, 255,
        0, 0, 255, 255,

        255, 0, 0, 255,
        255, 0, 0, 255,
        255, 0, 0, 255,
        255, 0, 0, 255,

        255, 255, 0, 255,
        255, 255, 0, 255,
        255, 255, 0, 255,
        255, 255, 0, 255,

        0, 255, 255, 255,
        0, 255, 255, 255,
        0, 255, 255, 255,
        0, 255, 255, 255
    };

    GLuint vboColors;
    glGenBuffers(1, &vboColors);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLubyte) * 4 * 4 * 6, colors, GL_STATIC_DRAW);

    GLuint color_attribute = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(color_attribute, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GLubyte) * 4, 0);
    glEnableVertexAttribArray(color_attribute);

    GLuint elements[] = {
        //Upper Face
        0, 1, 2, 2, 3, 0,
        //Lower Face
        4, 5, 6, 6, 7, 4,
        //Right Face
        8, 9, 10, 10, 11, 8,
        //Left Face
        12, 13, 14, 14, 15, 12,
        //Front Face
        16, 17, 18, 18, 19, 16,
        //Back Face
        20, 21, 22, 22, 23, 20,
    };

    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, elements, GL_STATIC_DRAW);
}

// Is called whenever a key is pressed/released via GLFW
void
KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "solar_system_data.cpp"
#include "mesh_utils.cpp"
#include "camera.cpp"
#include "shader.cpp"


bool activeDrawOrbits = true;
const GLuint WIDTH = 1600, HEIGHT = 900;
int current_time = 0;

void DrawSphere(glm::mat4 MVP, float* tint, GLuint program, Mesh* mesh);
void DrawOrbit(glm::mat4 MVP, GLuint program, Mesh* mesh);
void DrawSystem(glm::mat4 parent, SolarSystemDescription system, GLuint program, Mesh* sphereMesh, Mesh* orbitMesh);

int MVP_location, tint_location;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
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
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Compile and link the shader program:
    GLuint shaderProgram = Shader::LoadShader("shader/color.vert", "shader/color.frag");
    glUseProgram(shaderProgram);

    // Get the location of the uniform variables:
    MVP_location = glGetUniformLocation(shaderProgram, "MVP");
    tint_location = glGetUniformLocation(shaderProgram, "tint");

    // Set the depth test function:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Black background

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 
   
    // Create the meshes:
    Mesh mesh = Mesh_Utils::ColoredSphere();
    Mesh orbitMesh = Mesh_Utils::Orbit();

    // Create the camera object:
    Camera camera;
    camera.position = glm::vec3(100, 100, 100);
    camera.SetTarget(glm::vec3(0, 0, 0));
    camera.aspectRatio = WIDTH / HEIGHT;
    
    // God forgive me
    InitSystems();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // TODO: Port the camera controller

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        DrawSystem(camera.ViewProjectionMatrix(), System0, shaderProgram, &mesh, &orbitMesh);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return false;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void DrawSphere(glm::mat4 MVP, float* tint, GLuint program, Mesh* mesh)
{
    // glUseProgram(program);
    glUniformMatrix4fv(MVP_location, 1, false, glm::value_ptr(MVP)); // TODO: transpose?
    glUniformMatrix4fv(tint_location, 1, false, &tint[0]); // TODO: transpose?

    mesh->draw(GL_TRIANGLES);
}

void DrawOrbit(glm::mat4 MVP, GLuint program, Mesh* mesh)
{
    // glUseProgram(program);
    glUniformMatrix4fv(MVP_location, 1, false, glm::value_ptr(MVP)); // TODO: transpose?

    mesh->draw(GL_LINE_LOOP);
}

void DrawSystem(glm::mat4 parent, SolarSystemDescription system, GLuint program, Mesh* sphereMesh, Mesh* orbitMesh)
{
    glm::mat4 matPlanet = parent;

    // Translate and rotate around parent:
    matPlanet = glm::rotate(matPlanet, current_time * system.rotationSpeedAroundParent, glm::vec3(0, 1, 0));
    matPlanet = glm::translate(matPlanet, glm::vec3(system.distanceFromParent, 0, 0));
    matPlanet = glm::rotate(matPlanet, current_time * system.rotationSpeedAroundParent, glm::vec3(0, 1, 0));

    glm::mat4 matSystem = matPlanet;

    // Rotate around self:
    matPlanet = glm::rotate(matPlanet, current_time * system.rotationSpeedAroundSelf, glm::vec3(0, 1, 0));
    matPlanet = glm::scale(matPlanet, glm::vec3(system.scale, system.scale, system.scale));

    DrawSphere(matPlanet, system.tint, program, sphereMesh);
    if (activeDrawOrbits)
    {
        glm::mat4 matOrbit = parent;
        matOrbit = glm::scale(matOrbit, glm::vec3(system.distanceFromParent, 0, 0));
        DrawOrbit(matOrbit, program, orbitMesh);
    }

    // Draw all children:
    if (!system.children.empty())
    {
        for (auto& subSystem : system.children)
        {
            DrawSystem(matSystem, subSystem, program, sphereMesh, orbitMesh);
        }
    }
}

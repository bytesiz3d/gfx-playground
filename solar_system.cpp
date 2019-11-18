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
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh_utils.hpp"
#include "camera.hpp"
#include "shader.hpp"

namespace SS
{
    #include "solar_system_data.hpp"

    bool activeDrawOrbits = true;
    const GLuint WIDTH = 1280, HEIGHT = 720;
    int current_time = 0;

    void DrawSphere(glm::mat4 MVP, float* tint, Mesh* mesh);
    void DrawOrbit(glm::mat4 MVP, Mesh* mesh);
    void DrawSystem(glm::mat4 parent, SolarSystemDescription system, Mesh* sphereMesh, Mesh* orbitMesh);

    int MVP_location, tint_location;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    glm::vec3 movement(0, 0, 0);
    glm::vec2 mouseDelta(0, 0);

    void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    bool holdMouse = false;
};

int main()
{
    glfwInit();
    // Set all the required options for GLFW:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(SS::WIDTH, SS::HEIGHT, "Solar System", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, SS::key_callback);
    glfwSetMouseButtonCallback(window, SS::mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SS::WIDTH, SS::HEIGHT);

    // Compile and link the shader program:
    GLuint shaderProgram = Shader::LoadShader("shader/color.vert", "shader/color.frag");
    glUseProgram(shaderProgram);

    // Get the location of the uniform variables:
    SS::MVP_location = glGetUniformLocation(shaderProgram, "MVP");
    SS::tint_location = glGetUniformLocation(shaderProgram, "tint");

    // Set the depth test function:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Black background

    // Create the meshes:
    Mesh mesh = Mesh_Utils::ColoredSphere();
    Mesh orbitMesh = Mesh_Utils::Orbit();

    // Create the camera object:
    Camera camera;
    camera.position = glm::vec3(100, 100, 100);
    camera.SetTarget(glm::vec3(0, 0, 0));
    camera.aspectRatio = (float)SS::WIDTH / SS::HEIGHT;

    // #high_level_language
    SS::InitSystems();

    double p_mX, p_mY,
           mX,   mY;

    glfwGetCursorPos(window, &p_mX, &p_mY);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glfwGetCursorPos(window, &mX, &mY);
        SS::mouseDelta = glm::vec2(mX - p_mX, mY - p_mY);
        p_mX = mX, p_mY = mY;

        // TODO: Patch up the camera controller (qe, ad, ws)
        if (SS::holdMouse)
            camera.UpdateCamera(SS::mouseDelta, SS::movement);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        SS::DrawSystem(camera.ViewProjectionMatrix(), SS::System1, &mesh, &orbitMesh);
        glfwSwapBuffers(window);

        SS::current_time += 5;
    }
    glfwTerminate();
    return false;
}

// Is called whenever a key is pressed/released via GLFW
void SS::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            SS::movement[2] = 1;
            break;

        case GLFW_KEY_S:
            SS::movement[2] = -1;
            break;

        case GLFW_KEY_D:
            SS::movement[0] = 1;
            break;

        case GLFW_KEY_A:
            SS::movement[0] = -1;
            break;

        case GLFW_KEY_Q:
            SS::movement[1] = 1;
            break;

        case GLFW_KEY_E:
            SS::movement[1] = -1;
            break;

        default:
            break;
        }
    }

    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
        case GLFW_KEY_S:
            SS::movement[2] = 0;
            break;

        case GLFW_KEY_D:
        case GLFW_KEY_A:
            SS::movement[0] = 0;
            break;

        case GLFW_KEY_Q:
        case GLFW_KEY_E:
            SS::movement[1] = 0;
            break;

        default:
            break;
        }
    }
}

void SS::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            SS::holdMouse = true;
        else if (action == GLFW_RELEASE)
            SS::holdMouse = false;
    }
}

void SS::DrawSphere(glm::mat4 MVP, float* tint, Mesh* mesh)
{
    glUniformMatrix4fv(SS::MVP_location, 1, false, glm::value_ptr(MVP));
    glUniformMatrix4fv(SS::tint_location, 1, false, &tint[0]);

    mesh->draw(GL_TRIANGLES);
}

void SS::DrawOrbit(glm::mat4 MVP, Mesh* mesh)
{
    glUniformMatrix4fv(SS::MVP_location, 1, false, glm::value_ptr(MVP));

    mesh->draw(GL_LINE_LOOP);
}

void SS::DrawSystem(glm::mat4 parent, SolarSystemDescription system, Mesh* sphereMesh, Mesh* orbitMesh)
{
    glm::mat4 matPlanet = parent;

    // Translate and rotate around parent:
    matPlanet = glm::rotate(matPlanet, SS::current_time * system.rotationSpeedAroundParent, glm::vec3(0, 1, 0));
    matPlanet = glm::translate(matPlanet, glm::vec3(system.distanceFromParent, 0, 0));
    matPlanet = glm::rotate(matPlanet, SS::current_time * system.rotationSpeedAroundParent, glm::vec3(0, 1, 0));

    glm::mat4 matSystem = matPlanet;

    // Rotate around self:
    matPlanet = glm::rotate(matPlanet, SS::current_time * system.rotationSpeedAroundSelf, glm::vec3(0, 1, 0));
    matPlanet = glm::scale(matPlanet, glm::vec3(system.scale, system.scale, system.scale));

    DrawSphere(matPlanet, system.tint, sphereMesh);
    if (SS::activeDrawOrbits)
    {
        glm::mat4 matOrbit = parent;
        matOrbit = glm::scale(matOrbit, glm::vec3(system.distanceFromParent, system.distanceFromParent, system.distanceFromParent));
        DrawOrbit(matOrbit, orbitMesh);
    }

    // Draw all children:
    if (!system.children.empty())
    {
        for (auto& subSystem : system.children)
        {
            DrawSystem(matSystem, subSystem, sphereMesh, orbitMesh);
        }
    }
}

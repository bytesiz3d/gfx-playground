#pragma once
class Camera
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    float perspectiveFoVy;
    float aspectRatio;

    float near;
    float far;

    Camera()
    {
        position = glm::vec3(0, 0, 0);
        direction = glm::vec3(0, 0, 1);
        up = glm::vec3(0, 1, 0);

        perspectiveFoVy = glm::pi<float>() / 2;
        aspectRatio = 1;

        near = 0.01;
        far = 1000;
    }

    glm::mat4 ViewMatrix()
    {
        glm::vec3 center = position + direction;
        return glm::lookAt(position, center, up);
    }

    glm::mat4 ProjectionMatrix()
    {
        return glm::perspective(perspectiveFoVy, aspectRatio, near, far);
    }

    glm::mat4 ViewProjectionMatrix()
    {
        return ProjectionMatrix() * ViewMatrix();
    }

    void SetTarget(glm::vec3 target)
    {
        direction = target - position;
    }

    glm::vec3 right()
    {
        glm::vec3 n_up = glm::normalize(up);
        return glm::cross(direction, n_up);
    }
};

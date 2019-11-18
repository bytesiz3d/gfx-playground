#pragma once
class Camera
{
    float yaw;
    float pitch;

    float yawSens;
    float pitchSens;
    float movementSens;

    const float PI;

public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    float perspectiveFoVy;
    float aspectRatio;

    float near;
    float far;

    Camera()
        : yaw(0), pitch(0), yawSens(0.001), pitchSens(0.001), movementSens(0.001), PI(glm::pi<float>())
    {
        position = glm::vec3(0, 0, 0);
        direction = glm::vec3(0, 0, 1);
        up = glm::vec3(0, 1, 0);

        perspectiveFoVy = PI / 2;
        aspectRatio = 1;

        near = 0.01;
        far = 1000;

        auto dir = direction;
        auto len = dir[0] * dir[0];
        len += dir[1] * dir[1];
        len = std::sqrt(len);

        yaw = std::atan2(dir[2], dir[0]);
        pitch = std::atan2(dir[1], len);
    }

    void UpdateCamera(glm::vec2 mouseDelta, glm::vec3 movement)
    {
        yaw += mouseDelta[0] * yawSens;
        pitch += -mouseDelta[1] * pitchSens;

        pitch = glm::min(PI / 2, glm::max(-PI / 2, pitch));
        direction = glm::vec3(std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch));

        //movement *= movementSens;

        position += direction * movement[2];
        position += right() * movement[0];
        position += up * movement[1];
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
        direction = glm::normalize(direction);

        // Adjust pitch and yaw:
        auto dir = direction;
        auto len = dir[0] * dir[0];
        len += dir[1] * dir[1];
        len = std::sqrt(len);

        yaw = std::atan2(dir[2], dir[0]);
        pitch = std::atan2(dir[1], len);
    }

    glm::vec3 right()
    {
        glm::vec3 n_up = glm::normalize(up);
        return glm::cross(direction, n_up);
    }
};

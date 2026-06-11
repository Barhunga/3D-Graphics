#pragma once

#include <glm/ext.hpp>
struct GLFWwindow;

class Camera
{
public:

    Camera();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float w, float h);

    glm::vec3 getPosition() { return m_position; }

    void update(float deltaTime, GLFWwindow* window);

private:

    float m_theta;
    float m_phi;
    glm::vec3 m_position;
};


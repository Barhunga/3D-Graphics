#include "Camera.h"

#include <GLFW/glfw3.h>
#include "Application3D.h"

Camera::Camera() : m_position(-23,5,0), m_theta(0), m_phi(0)
{
    
}

glm::mat4 Camera::getViewMatrix()
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::pi<float>() * 0.25f,
                            w / h,
                            0.1f, 1000.f);
}

void Camera::update(float deltaTime, GLFWwindow* window)
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    //calculate the forwards and right axes and up axis for the camera
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    glm::vec3 up(0, 1, 0);

    // use WASD, ZX keys to move camera around
    if (glfwGetKey(window, GLFW_KEY_X))
        m_position += up * deltaTime * 10.f;
    if (glfwGetKey(window, GLFW_KEY_Z))
        m_position -= up * deltaTime * 10.f;
    if (glfwGetKey(window, GLFW_KEY_W))
        m_position += forward * deltaTime * 10.f;
    if (glfwGetKey(window, GLFW_KEY_A))
        m_position -= right * deltaTime * 10.f;
    if (glfwGetKey(window, GLFW_KEY_S))
        m_position -= forward * deltaTime * 10.f;
    if (glfwGetKey(window, GLFW_KEY_D))
        m_position += right * deltaTime * 10.f;

    glm::vec2 mouseDelta = Application3D::get()->getMouseDelta();

    const float turnSpeed = 0.2f;
    // if the right button is down, increment theta and phi
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        // hide and confine cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // rotate camera
        m_theta += turnSpeed * mouseDelta.x;
        m_phi -= turnSpeed * mouseDelta.y;
        // clamp to avoid gimbal lock
        if (m_phi > 80) m_phi = 80;
        if (m_phi < -80) m_phi = -80;
    }
    else
    {
        // unhide cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"

#define IS_KEY_DOWN(key) glfwGetKey(win, key) == GLFW_PRESS


Camera::Camera(GLFWwindow *win, float aspect)
    : m_aspect(aspect)
{
    m_speed = 3.5f;
    m_sensitivity = 0.07f;

    m_fov = 60.0f;
    m_near = 0.01f;
    m_far = 1000.0f;

    yaw = -90.0f;
    pitch = 0.0f;
    position = {0.0, 0.0, 3.5};
    m_front = {0.0, 0.0, -1.0};
    m_up = {0.0, 1.0, 0.0};
    m_right = {1.0, 0.0, 0.0};
    m_view = glm::mat4(1.0);
    m_projection = glm::mat4(1.0);
    view_projection = glm::mat4(1.0);

    // matrices setup
    glfwGetCursorPos(win, &m_cursorX, &m_cursorY);
    // m_view = glm::translate(m_view, position);
    m_view = glm::lookAtRH(position, position+m_front, m_up);
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    view_projection = m_projection * m_view;
}

Camera::~Camera() {

}

void Camera::update_aspect(float new_aspect) {
    m_aspect = new_aspect;
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    view_projection = m_projection * m_view;
}

void Camera::handle_input(GLFWwindow *win, float delta_time) {
    double cursorX, cursorY;
    glfwGetCursorPos(win, &cursorX, &cursorY);
    float offsetX = cursorX - m_cursorX;
    float offsetY = m_cursorY - cursorY;
    m_cursorX = cursorX;
    m_cursorY = cursorY;
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;
    yaw   += offsetX;
    pitch += offsetY;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    glm::vec3 direction;
    direction[0] = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction[1] = sin(glm::radians(pitch));
    direction[2] = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front = glm::normalize(direction);
    m_right = glm::cross({0.0, 1.0, 0.0}, m_front);
    m_up    = glm::cross(m_front, m_right);
    
    // handle keyboard
    // the IS_KEY_DOWN macro needs a variable of type GLFWwindow& named win
    glm::vec3 movement(0.0f);
    if (IS_KEY_DOWN(GLFW_KEY_W)) {
        movement += m_front * 1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_S)) {
        movement += m_front * -1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_A)) {
        movement += m_right * 1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_D)) {
        movement += m_right * -1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_SPACE)) {
        movement += glm::vec3(0, 1.0f, 0) * 1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_LEFT_CONTROL)) {
        movement += glm::vec3(0, 1.0f, 0) * -1.0f;
    }

    // handle math 
    if (glm::length(movement) != 0.0)
        movement = glm::normalize(movement);
    glm::vec3 movement_delta = movement * -m_speed * delta_time;
    position += movement_delta * -1.0f; // debuging for now
    m_view = glm::lookAtRH(position, position+m_front, m_up);
    view_projection = m_projection * m_view;
}

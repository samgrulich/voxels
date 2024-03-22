#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

#include "Camera.h"

#define IS_KEY_DOWN(key) glfwGetKey(win, key) == GLFW_PRESS


Camera::Camera(GLFWwindow *win, float aspect)
    : aspect_(aspect)
{
    speed_ = 14.f;
    sensitivity_ = 0.07f;

    fov_ = 60.0f;
    near_ = 0.01f;
    far_ = 1000.0f;

    yaw = 90.0f;
    pitch = 0.0f;
    position = {16.0, 70.0, 16.0};
    front_ = {0.0, 0.0, 1.0};
    up_ = {0.0, 1.0, 0.0};
    right_ = {1.0, 0.0, 0.0};
    view_ = glm::mat4(1.0);
    projection_ = glm::mat4(1.0);
    viewProjection = glm::mat4(1.0);

    // matrices setup
    glfwGetCursorPos(win, &cursorX_, &cursorY_);
    view_ = glm::lookAtRH(position, position+front_, up_);
    projection_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
    viewProjection = projection_ * view_;
}

Camera::~Camera() {

}

void Camera::updateAspect(float newAspect) {
    aspect_ = newAspect;
    projection_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
    viewProjection = projection_ * view_;
}

void Camera::handleInput(GLFWwindow *win, float deltaTime) {
    double cursorX, cursorY;
    glfwGetCursorPos(win, &cursorX, &cursorY);
    float offsetX = cursorX - cursorX_;
    float offsetY = cursorY_ - cursorY;
    if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
        offsetX = 0.0f;
        offsetY = 0.0f; 
    }
    cursorX_ = cursorX;
    cursorY_ = cursorY;
    offsetX *= sensitivity_;
    offsetY *= sensitivity_;
    yaw   += offsetX;
    pitch += offsetY;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    glm::vec3 direction;
    direction[0] = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction[1] = sin(glm::radians(pitch));
    direction[2] = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_ = glm::normalize(direction);
    right_ = glm::cross({0.0, 1.0, 0.0}, front_);
    up_    = glm::cross(front_, right_);
    
    // handle keyboard
    // the IS_KEY_DOWN macro needs a variable of type GLFWwindow& named win
    glm::vec3 movement(0.0f);
    if (IS_KEY_DOWN(GLFW_KEY_W)) {
        movement += front_ * 1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_S)) {
        movement += front_ * -1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_A)) {
        movement += right_ * 1.0f;
    }
    if (IS_KEY_DOWN(GLFW_KEY_D)) {
        movement += right_ * -1.0f;
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
    glm::vec3 movement_delta = movement * -speed_ * deltaTime;
    position += movement_delta * -1.0f; // debuging for now
    view_ = glm::lookAtRH(position, position+front_, up_);
    viewProjection = projection_ * view_;
}

glm::vec3& Camera::front() {
    return front_;
}

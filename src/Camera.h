#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    public:
        float yaw, pitch;
        glm::vec3 position;
        glm::mat4 viewProjection;
        double cursorX_, cursorY_;
    private:
        float speed_, sensitivity_;
        float fov_, near_, far_, aspect_;
        glm::vec3 front_, up_, right_;
        glm::mat4 view_, projection_;
    public: 
        Camera(GLFWwindow *win, float aspect);
        ~Camera();

        void updateAspect(float newAspect);
        void handleInput(GLFWwindow *win, float deltaTime);
        glm::vec3 front();
};

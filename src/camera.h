#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    public:
        float yaw, pitch;
        glm::vec3 position;
        glm::mat4 view_projection;
        double m_cursorX, m_cursorY;
    private:
        float m_speed, m_sensitivity;
        float m_fov, m_near, m_far, m_aspect;
        glm::vec3 m_front, m_up, m_right;
        glm::mat4 m_view, m_projection;
    public: 
        Camera(GLFWwindow *win, float aspect);
        ~Camera();

        void update_aspect(float new_aspect);
        void handle_input(GLFWwindow *win, float delta_time);
};

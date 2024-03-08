#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
    public:
        glm::vec3 position;
        float yaw, pitch;
    private:
        float speed, sensitivity;
        float fov, near, far, aspect;
        glm::vec3 front, up, right;
        glm::mat4 view, projection, view_projection;
    public: 
        Camera();
        ~Camera();

        void update_aspect();
        void handle_input();
};

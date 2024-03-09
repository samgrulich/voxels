#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "shader.h"
#include "shape.h"


static struct State {
    GLFWwindow* win;
    Camera* cam;
} state;

// glfw callbacks
void resize_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void) {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, false);
    glm::ivec2 win_size(800, 600);
    state.win = glfwCreateWindow(win_size.x, win_size.y, "Hola", NULL, NULL);
    if (!state.win) {
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(state.win, key_callback);
    // glfwSetWindowSizeCallback(state.win, resize_callback);
    glfwSetFramebufferSizeCallback(state.win, resize_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(state.win);
    if(glewInit() != GLEW_OK) {
        std::cerr << "Error: Couldn't initalize OpenGL." << std::endl;
        return 1;
    }

    // imgui init 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(state.win, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // my init
    Camera cam(state.win, (float)win_size.x/win_size.y);
    ShaderProgram basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    Shape quadShape = cube(1.0);
    glm::vec4 clearColor = {0.046, 0.046, 0.064, 1.0};

    state.cam = &cam;
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(state.win)) {
        cam.handle_input(state.win, 1.0/60.0);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (ImGui::ColorPicker4("Clear color", &clearColor.x)) {
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        }
        ImGui::DragFloat3("Position", &cam.position.x, 0.1f);


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        basicShader.bind();
        basicShader.set("u_color", 1.0f, 1.0f, 0.0f);
        basicShader.set("u_MVP", cam.view_projection);
        quadShape.draw();

        // IMGUI Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(state.win);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void resize_callback(GLFWwindow* win, int width, int height) {
    glfwGetCursorPos(state.win, &state.cam->m_cursorX, &state.cam->m_cursorY);
    state.cam->update_aspect((float)width / height);
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    static bool cursor_hidden = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (cursor_hidden) {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwGetCursorPos(win, &state.cam->m_cursorX, &state.cam->m_cursorY);
        } else {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        cursor_hidden = !cursor_hidden;
    }
}

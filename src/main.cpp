#include <iostream>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Chunk.h"
#include "WorldConstants.h"

#include "tracy/Tracy.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


static struct State {
    GLFWwindow* win;
    glm::ivec2 winSize = {800, 600};
    Camera* cam;
    bool drawLines = false;
    bool cullFaces = true;
    bool shouldWindowClose = false;
} s_state;

// glfw callbacks
void resizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// my funcs
void changeDrawMode() {
    if (s_state.drawLines) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main(void) {
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_SAMPLES, 4);
    s_state.win = glfwCreateWindow(s_state.winSize.x, s_state.winSize.y, "Development", NULL, NULL);
    if (!s_state.win) {
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(s_state.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(s_state.win, keyCallback);
    // glfwSetWindowSizeCallback(state.win, resize_callback);
    glfwSetFramebufferSizeCallback(s_state.win, resizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(s_state.win);
    glfwSwapInterval(1);
    // glfwSwapInterval(0);
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
    ImGui_ImplGlfw_InitForOpenGL(s_state.win, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    
    // stbi setup
    stbi_set_flip_vertically_on_load(true);

    // my init
    Camera cam(s_state.win, (float)s_state.winSize.x/s_state.winSize.y);
    cam.position = {14.5f, 15.0f, -16.0f};
    ShaderProgram basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    glm::vec4 clearColor = {0.025, 0.770, 1.000, 1.0};
    glm::vec3 lightDir = {0.5f, 1.0f, 0.7f};

    s_state.cam = &cam;
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    if (s_state.cullFaces)
        glEnable(GL_CULL_FACE);
    if (s_state.drawLines)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // initialize opengl
    int chunkSize = 8;
    std::vector<Chunk*> chunks;
    auto terrainStart     = std::chrono::steady_clock::now();
    {
        ZoneScopedN("Terrain Generation");
        Block block = {1, true};
        for (int z1 = 0; z1 < chunkSize; z1++) {
            for (int x1 = 0; x1 < chunkSize; x1++) {
                for (int z = 0; z < Consts::CHUNK_SIZE; z++) {
                    for (int x = 0; x < Consts::CHUNK_SIZE; x++) {
                        World::setBlock(x+x1*Consts::CHUNK_SIZE, 0, z+z1*Consts::CHUNK_SIZE, block);
                    }
                }
            }
        }
    }
    auto terrainEnd = std::chrono::steady_clock::now();
    {
        ZoneScopedN("Mesh Generation");
        for (int z1 = 0; z1 < chunkSize; z1++) {
            for (int x1 = 0; x1 < chunkSize; x1++) {
                Chunk* chunk = new Chunk({x1*Consts::CHUNK_SIZE, 0, z1*Consts::CHUNK_SIZE});
                chunk->remesh();
                chunks.push_back(chunk);
            }
        }
    }
    auto meshEnd = std::chrono::steady_clock::now();
    std::cout << "Terrain generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(terrainEnd - terrainStart).count() << "ms" << std::endl;
    std::cout << "Mesh generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(meshEnd - terrainEnd).count() << "ms" << std::endl;
    
    Texture texture("res/dev.jpg", GL_RGB);
    basicShader.bind();
    basicShader.set("u_texture", 0);
    basicShader.set("u_lightDir", lightDir);


    auto start     = std::chrono::steady_clock::now();
    auto lastFrame = std::chrono::steady_clock::now();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(s_state.win)) {
        FrameMark;
        ZoneScopedN("Main Loop");
        auto thisFrame  = std::chrono::steady_clock::now();
        float frameTime = std::chrono::duration_cast<std::chrono::microseconds>(thisFrame - lastFrame).count() * 0.001;
        float deltaTime = frameTime * 0.001;
        lastFrame = std::chrono::steady_clock::now();

        cam.handleInput(s_state.win, deltaTime);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Text("FPS: %.2f %.2fms", 1000.0/frameTime, frameTime);
        if (ImGui::ColorEdit3("Clear color", &clearColor.x)) {
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        }
        if (ImGui::Checkbox("Draw lines", &s_state.drawLines)) {
            changeDrawMode();
        }
        ImGui::DragFloat3("Position", &cam.position.x, 0.1f);


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.bind(0);

        basicShader.bind();
        basicShader.set("u_color", 1.0f, 1.0f, 0.0f);
        basicShader.set("u_MVP", cam.viewProjection);

        // rectangle.draw();
        // mesh.draw();
        for (Chunk* chunk : chunks) {
            chunk->draw();
        }

        basicShader.refresh();

        // IMGUI Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(s_state.win);

        /* Poll for and process events */
        glfwPollEvents();
        glm::ivec2 winSize;
        glfwGetWindowSize(s_state.win, &winSize.x, &winSize.y);
        if (s_state.winSize != winSize) {
            glfwGetCursorPos(s_state.win, &cam.cursorX_, &cam.cursorY_);
            s_state.winSize = winSize;
        }
    }

    s_state.shouldWindowClose = true;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void resizeCallback(GLFWwindow* win, int width, int height) {
    s_state.cam->updateAspect((float)width / height);
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    static bool cursor_hidden = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (cursor_hidden) {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwGetCursorPos(win, &s_state.cam->cursorX_, &s_state.cam->cursorY_);
        } else {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        cursor_hidden = !cursor_hidden;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        s_state.drawLines = !s_state.drawLines;
        changeDrawMode();
    }
}

#include <iostream>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <thread>

#include "Camera.h"
#include "Shader.h"
#include "Chunk.h"
#include "ChunkManager.h"

/* TODOS:
 *  optimize chunk generation/meshing
 *  fix segfault on loading and unloading simultaneously mulitple regions (missing check)
 *  fix not unloading chunks properly
 *  fix chunk generation (meshes between chunks)
 *  ? frustum chunk generation
 */

static struct State {
    GLFWwindow* win;
    glm::ivec2 winSize = {800, 600};
    Camera* cam;
    bool drawLines = true;
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

void startChunkGeneration(ChunkManager *world) {
    using namespace std::chrono_literals;
    while (!s_state.shouldWindowClose) {
        world->generateChunks();
        std::this_thread::sleep_for(250ms);
    }
}

void startChunkMeshing(ChunkManager *world) {
    using namespace std::chrono_literals;
    while (!s_state.shouldWindowClose) {
        world->meshChunks(World::CHUNK_TOLOAD_BATCH);
        std::this_thread::sleep_for(250ms);
    }
}

int main(void) {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_SAMPLES, 4);
    s_state.win = glfwCreateWindow(s_state.winSize.x, s_state.winSize.y, "Hola", NULL, NULL);
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

    // my init
    Camera cam(s_state.win, (float)s_state.winSize.x/s_state.winSize.y);
    // ShaderProgram basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    ShaderProgram cubeShader("res/shaders/cube.vert", "res/shaders/cube.frag");
    ChunkManager world;
    glm::vec4 clearColor = {0.025, 0.770, 1.000, 1.0};

    s_state.cam = &cam;
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // std::thread t1(startChunkGeneration, &world);
    // std::thread t2(startChunkMeshing, &world);
    auto start = std::chrono::steady_clock::now();
    auto lastFrame = std::chrono::steady_clock::now();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(s_state.win)) {
        auto thisFrame = std::chrono::steady_clock::now();
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
        // basicShader.bind();
        // basicShader.set("u_color", 1.0f, 1.0f, 0.0f);
        // basicShader.set("u_MVP", cam.viewProjection);
        cubeShader.bind();
        // cubeShader.set("u_color", 1.0f, 1.0f, 1.0f);
        cubeShader.set("u_MVP", cam.viewProjection);

        world.generateChunks();
        world.meshChunks(10);
        world.drawChunks(cam.position, cam.front(), cubeShader);
        // world.meshChunks();
        // basicShader.refresh();
        cubeShader.refresh();

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
    // t1.join();
    // t2.join();
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

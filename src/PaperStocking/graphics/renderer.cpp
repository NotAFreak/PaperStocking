#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>
#include <PaperStocking/engine.hpp>
#include <PaperStocking/debug/timer.hpp>
#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/settings/settingsManager.hpp>
using namespace PaperStocking;
Renderer::Renderer() {
    assert(s_Instance == nullptr);
    s_Instance = this;

    s_Instance->m_OutputResolution.x = SettingsManager::GetSetting("Graphics", "Width", 1280);
    s_Instance->m_OutputResolution.y = SettingsManager::GetSetting("Graphics", "Height", 720);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Couldn't initialize SDL!: " + std::string(SDL_GetError()));
    }
    m_Window = SDL_CreateWindow(Engine::info.m_ApplicationName.c_str(),
        s_Instance->m_OutputResolution.x, s_Instance->m_OutputResolution.y,
        SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );
    if (m_Window == nullptr) {
        throw std::runtime_error("Couldn't create SDL window!: " + std::string(SDL_GetError()));
    }
    m_GlContext = SDL_GL_CreateContext(m_Window);
    if (!m_GlContext) {
        throw std::runtime_error("Failed to create GL Context: " + std::string(SDL_GetError()));
    }
    if (!gladLoadGL(SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD: " + std::string(SDL_GetError()));
    }
    SetRenderScale(SettingsManager::GetSetting("Graphics", "RenderScale", 1.0f));
    // V-Sync
    SDL_GL_SetSwapInterval(SettingsManager::GetSetting("Graphics", "Vsync", 1));

    // GL shit
    glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);

    // Camera data
    glGenBuffers(1, &camDataUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, camDataUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camDataUBO);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(m_Window, m_GlContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    m_DebugUi = std::make_unique<DebugUiManager>();
}

Renderer::~Renderer() {
    assert(s_Instance != nullptr);
    m_DebugUi.reset();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(m_GlContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();

    s_Instance = nullptr;
}

void Renderer::Render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    UploadFrameData();
    // Render
    Timer::Begin("Debug UI");
    m_DebugUi->Render();
    Timer::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_Window);
}

// GPU Calls
const glm::vec4 Renderer::GetClearColor() {
    assert(s_Instance != nullptr);
    return s_Instance->m_ClearColor;
}
void Renderer::SetClearColor(glm::vec4 color) {
    assert(s_Instance != nullptr);
    s_Instance->m_ClearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}
void Renderer::SetOutputResolution(glm::uvec2 resolution, bool setWindow) {
    assert(s_Instance != nullptr);
    s_Instance->m_OutputResolution = resolution;
    s_Instance->m_RenderResolution = glm::uvec2 {
        static_cast<uint>(static_cast<float>(s_Instance->m_OutputResolution.x) * s_Instance->m_RenderScale),
        static_cast<uint>(static_cast<float>(s_Instance->m_OutputResolution.y) * s_Instance->m_RenderScale)
    };
    glViewport(0, 0, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y);
}
void Renderer::SetRenderScale(float scale) {
    assert(s_Instance != nullptr);
    s_Instance->m_RenderScale = scale;
    s_Instance->m_RenderResolution = glm::uvec2 {
        static_cast<uint>(static_cast<float>(s_Instance->m_OutputResolution.x) * s_Instance->m_RenderScale),
        static_cast<uint>(static_cast<float>(s_Instance->m_OutputResolution.y) * s_Instance->m_RenderScale)
    };
    glViewport(0, 0, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y);
}


//  Private
void Renderer::UploadFrameData() {
    assert(s_Instance != nullptr);
    {   // Upload camera data
        glm::mat4 camMat = s_Instance->camera.GetFullMatrix();
        glm::vec3 camPos = s_Instance->camera.transform.GetGlobalPosition();
        glm::vec2 screenSize = s_Instance->m_RenderResolution;

        glBindBuffer(GL_UNIFORM_BUFFER, s_Instance->camDataUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camMat));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(camPos));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec2), glm::value_ptr(screenSize));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

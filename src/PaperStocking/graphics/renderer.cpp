#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

#include <PaperStocking/engine.hpp>
#include <PaperStocking/debug/timer.hpp>
#include <PaperStocking/input/rawInput.hpp>
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
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    s_Window = SDL_CreateWindow(Engine::info.m_ApplicationName.c_str(),
        s_Instance->m_OutputResolution.x, s_Instance->m_OutputResolution.y,
        SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );
    if (s_Window == nullptr) {
        throw std::runtime_error("Couldn't create SDL window!: " + std::string(SDL_GetError()));
    }
    m_GlContext = SDL_GL_CreateContext(s_Window);
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
    size_t totalSize = sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(glm::vec2);
    glBufferData(GL_UNIFORM_BUFFER,totalSize,nullptr,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camDataUBO);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(s_Window, m_GlContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    m_UiRenderInterface = new RenderInterface_GL3();
    m_UiRenderInterface->SetViewport(m_OutputResolution.x, m_OutputResolution.y);
    Rml::SetRenderInterface(m_UiRenderInterface);
    m_UiSystemInterface = new SystemInterface_SDL(s_Window);
    Rml::SetSystemInterface(m_UiSystemInterface);

    Rml::Initialise();
    m_UiManager.Initialize();

    m_DebugUi = std::make_unique<DebugUiManager>();

    RawInput::SubscribeListener(EventListener);
}

Renderer::~Renderer() {
    assert(s_Instance != nullptr);
    m_UiManager.Shutdown();
    Rml::Shutdown();
    delete m_UiRenderInterface;
    m_DebugUi.reset();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(m_GlContext);
    SDL_DestroyWindow(s_Window);
    SDL_Quit();
    RawInput::UnsubscribeListener(EventListener);

    s_Instance = nullptr;
}

void Renderer::Render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    m_UiManager.Update();
    UiManager::ExecuteAllEvents(UiExecThread::Render);

    Timer::Begin("Upload Frame Data");
    UploadFrameData();
    Timer::End();

    // Actual Render
    Timer::Begin("UI");
    m_UiRenderInterface->BeginFrame();
    m_UiManager.Render();
    m_UiRenderInterface->EndFrame();
    Timer::End();

    Timer::Begin("Debug UI");
    m_DebugUi->Render();
    Timer::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(s_Window);
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
    s_Instance->m_UiRenderInterface->SetViewport(s_Instance->m_OutputResolution.x, s_Instance->m_OutputResolution.y);
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

glm::uvec2 Renderer::GetOutputResolution() {
    assert(s_Instance != nullptr);
    return s_Instance->m_OutputResolution;
}

glm::uvec2 Renderer::GetRenderResolution() {
    assert(s_Instance != nullptr);
    return s_Instance->m_RenderResolution;
}

float Renderer::GetRenderScale() {
    assert(s_Instance != nullptr);
    return s_Instance->m_RenderScale;
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

void Renderer::EventListener(SDL_Event& event) {
    if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        int w, h;
        SDL_GetWindowSize(s_Window, &w, &h);
        SetOutputResolution({w, h}, false);
    }
}

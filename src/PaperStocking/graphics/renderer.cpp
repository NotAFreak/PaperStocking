#include <PaperStocking/graphics/renderer.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>
using namespace PaperStocking;

Renderer::Renderer() {
    assert(instance == nullptr);
    instance = this;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Couldn't initialize SDL!: " + std::string(SDL_GetError()));
    }
    m_Window = SDL_CreateWindow("PaperStocking Window", 1280, 720,
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

    // V-Sync
    SDL_GL_SetSwapInterval(1);

    // GL shit
    glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);


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
    assert(instance != nullptr);
    m_DebugUi.reset();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(m_GlContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();

    instance = nullptr;
}

void Renderer::Render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    // Render
    m_DebugUi->Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_Window);
}

// GPU Calls
const glm::vec4 Renderer::GetClearColor() {
    assert(instance != nullptr);
    return instance->m_ClearColor;
}

void Renderer::SetClearColor(glm::vec4 color) {
    assert(instance != nullptr);
    instance->m_ClearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

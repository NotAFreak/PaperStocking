#include <entt/entity/entity.hpp>
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
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_WindowFlags windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    if (SettingsManager::GetSetting("Graphics", "HighDPI", true)) {
        windowFlags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
    }

    s_Window = SDL_CreateWindow(Engine::info.m_ApplicationName.c_str(),
        s_Instance->m_OutputResolution.x, s_Instance->m_OutputResolution.y,
        windowFlags
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
    glGenBuffers(1, &m_CamDataUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_CamDataUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(m_CamData),nullptr,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CamDataUBO);

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

    // Create GBuffer
    {
        // Create GBuffer
        glGenFramebuffers(1, &m_GBuffer.frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer.frameBuffer);

        glGenTextures(1, &m_GBuffer.color);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.color);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_RenderResolution.x, m_RenderResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GBuffer.color, 0);

        glGenTextures(1, &m_GBuffer.position);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.position);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_RenderResolution.x, m_RenderResolution.y, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GBuffer.position, 0);

        glGenTextures(1, &m_GBuffer.norm);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.norm);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_RenderResolution.x, m_RenderResolution.y, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GBuffer.norm, 0);

        glGenTextures(1, &m_GBuffer.lighting);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.lighting);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_RenderResolution.x, m_RenderResolution.y, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_GBuffer.lighting, 0);

        glGenRenderbuffers(1, &m_GBuffer.depth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_GBuffer.depth);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            m_RenderResolution.x,
            m_RenderResolution.y
        );
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_GBuffer.depth);

        // Bind regular GBuffer again
        unsigned int attachments[4] = {
            GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
        };
        glDrawBuffers(4, attachments);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDepthFunc(GL_LEQUAL);

        // Create Depth Frame buffer
        glGenFramebuffers(1, &m_GBuffer.depthFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer.depthFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_GBuffer.depthFrameBuffer);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            m_RenderResolution.x,
            m_RenderResolution.y
        );
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_GBuffer.depth);
        glDepthFunc(GL_LEQUAL);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_GBuffer.gBufferShader = ShaderManager::GetShaderProgram("gbuffer");
    }

    // Create quad
    {
        SpriteMeshGPUData gpuData;
        float vertices[] = {
            // Positions            // TexCoords (UV)
            -1.0f, -1.0f, 0.0f,     0.0f, 0.0f, // Bottom Left
            -1.0f,  1.0f, 0.0f,     0.0f, 1.0f, // Top Left
             1.0f, -1.0f, 0.0f,     1.0f, 0.0f, // Bottom Right

            -1.0f,  1.0f, 0.0f,     0.0f, 1.0f, // Top Left
             1.0f,  1.0f, 0.0f,     1.0f, 1.0f, // Top Right
             1.0f, -1.0f, 0.0f,     1.0f, 0.0f  // Bottom Right
        };
        glGenVertexArrays(1, &gpuData.m_VAO);
        glGenBuffers(1, &gpuData.m_VBO);

        glBindVertexArray(gpuData.m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, gpuData.m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        unsigned int stride = 5 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, false, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_QuadData = gpuData;
    }
    glCullFace(GL_BACK);
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
    Timer::Begin("Begin Frame");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer.frameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    m_UiManager.Update();
    Timer::Begin("Execute UI Events");
    UiManager::ExecuteAllEvents(UiExecThread::Render);
    Timer::End();

    ShaderManager::CompileFromQueues();

    Timer::Begin("Unload textures");
    {
        std::lock_guard lock(m_TextureUnloadQueue_mutex);
        while(!m_TextureUnloadQueue.empty()) {
            std::shared_ptr<Texture> texture = m_TextureUnloadQueue.front();
            texture->Unload();
            m_TextureUnloadQueue.pop();
        }
    }
    Timer::End();
    Timer::Begin("Upload textures");
    {
        std::lock_guard lock(m_TextureUploadQueue_mutex);
        while(!m_TextureUploadQueue.empty()) {
            std::shared_ptr<Texture> texture = m_TextureUploadQueue.front();
            texture->Upload();
            m_TextureUploadQueue.pop();
        }
    }
    Timer::End();

    Timer::Begin("Upload Frame Data");
    UploadFrameData();
    Timer::End();
    Timer::End();

    // Actual Render
    Timer::Begin("Render Frame");
    Timer::Begin("Sprites");
    glEnable(GL_DEPTH_TEST);
    {
        std::lock_guard lock(s_Instance->m_Sprites_mutex);
        for (auto& sprite : m_Sprites) {
            sprite->Render();
        }
    }
    Timer::End();
    Timer::Begin("Combine GBuffer");
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(m_GBuffer.gBufferShader->handle);
        SetUniformI(m_GBuffer.gBufferShader->handle, "gColor", 0);
        SetUniformI(m_GBuffer.gBufferShader->handle, "gPos", 1);
        SetUniformI(m_GBuffer.gBufferShader->handle, "gNorm", 2);
        SetUniformI(m_GBuffer.gBufferShader->handle, "gLighting", 3);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.color);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.position);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.norm);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_GBuffer.lighting);
        glBindRenderbuffer(GL_RENDERBUFFER, m_GBuffer.depth);

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(m_QuadData.m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }
    Timer::End();

    Timer::Begin("UI");
    {
        m_UiRenderInterface->BeginFrame();
        m_UiManager.Render();
        m_UiRenderInterface->EndFrame();
    }
    Timer::End();

    Timer::Begin("Debug UI");
    m_DebugUi->Render();
    Timer::End();
    Timer::End();

    Timer::Begin("End Frame");
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(s_Window);
    }
    Timer::End();
}

Camera &Renderer::GetCamera() {
    assert(s_Instance != nullptr);
    return s_Instance->m_Camera;
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
    ResizeColorAttachment(s_Instance->m_GBuffer.color, GL_RGBA8, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGBA);
    ResizeColorAttachment(s_Instance->m_GBuffer.position, GL_RGB16F, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGB);
    ResizeColorAttachment(s_Instance->m_GBuffer.norm, GL_RGB16F, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGB);
    ResizeColorAttachment(s_Instance->m_GBuffer.lighting, GL_RGB16F, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGB);
    glBindRenderbuffer(GL_RENDERBUFFER, s_Instance->m_GBuffer.depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y);
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
    ResizeColorAttachment(s_Instance->m_GBuffer.color, GL_RGBA8, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGBA);
    ResizeColorAttachment(s_Instance->m_GBuffer.position, GL_RGB16F, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGB);
    ResizeColorAttachment(s_Instance->m_GBuffer.norm, GL_RGB16F, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGB);
    ResizeColorAttachment(s_Instance->m_GBuffer.lighting, GL_RGB16F, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y, GL_UNSIGNED_BYTE, GL_RGB);
    glBindRenderbuffer(GL_RENDERBUFFER, s_Instance->m_GBuffer.depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y);
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
        glm::mat4 camMat = s_Instance->m_Camera.GetFullMatrix();
        glm::vec3 camPos = s_Instance->m_Camera.transform.GetGlobalPosition();
        glm::vec2 screenSize = {s_Instance->m_RenderResolution.x, s_Instance->m_RenderResolution.y};
        s_Instance->m_CamData = {
            camMat,
            camPos
        };

        glBindBuffer(GL_UNIFORM_BUFFER, s_Instance->m_CamDataUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(s_Instance->m_CamData), &s_Instance->m_CamData);
        // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camMat));
        // glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(camPos));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void Renderer::QueueTextureUpload(std::shared_ptr<Texture> texture) {
    assert(s_Instance != nullptr);
    std::lock_guard lock(s_Instance->m_TextureUploadQueue_mutex);
    s_Instance->m_TextureUploadQueue.push(texture);
}
void Renderer::QueueTextureUnload(std::shared_ptr<Texture> texture) {
    assert(s_Instance != nullptr);
    std::lock_guard lock(s_Instance->m_TextureUnloadQueue_mutex);
    s_Instance->m_TextureUnloadQueue.push(texture);
}

void Renderer::RegisterSpriteRenderable(std::shared_ptr<SpriteRenderable> sprite) {
    assert(s_Instance != nullptr);
    std::lock_guard lock(s_Instance->m_Sprites_mutex);
    s_Instance->m_Sprites.push_back(sprite);
}

void Renderer::UnregisterSpriteRenderable(std::shared_ptr<SpriteRenderable> sprite) {
    assert(s_Instance != nullptr);
    std::lock_guard lock(s_Instance->m_Sprites_mutex);
    std::vector<std::shared_ptr<SpriteRenderable>>::iterator it;
    for (it = s_Instance->m_Sprites.begin(); it != s_Instance->m_Sprites.end(); ++it) {
        if (it->get() == sprite.get()) break;
    }
    // assert(it != s_Instance->m_Sprites.end());
    s_Instance->m_Sprites.erase(it);
}

void Renderer::EventListener(SDL_Event& event) {
    if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        int w, h;
        SDL_GetWindowSize(s_Window, &w, &h);
        SetOutputResolution({w, h}, false);
    }
}

struct UniformWarning {
    uint handle;
    std::string name;
    bool operator==(const UniformWarning& other) const {
        return handle == other.handle && name == other.name;
    }
};
std::vector<UniformWarning> warnings;

bool WarningExists(uint handle, std::string name) {
    UniformWarning warning = UniformWarning {
        handle,
        name
    };
    std::vector<UniformWarning>::iterator it;
    for (it = warnings.begin(); it != warnings.end(); it++) {
        if (*it == warning) {
            return true;
        }
    }
    warnings.push_back(warning);
    return false;
}

void Renderer::SetUniformM4(uint shaderHandle, std::string name, glm::mat4 value) {
    assert(s_Instance != nullptr);
    int modelUniform = glGetUniformLocation(shaderHandle, name.c_str());
    if(modelUniform == -1) {
        if (WarningExists(shaderHandle, name)) return;
        s_Instance->m_Logger.Warning("Couldn't find uniform: " + name);
    }
    glUniformMatrix4fv(modelUniform, 1, false, &value[0][0]);
}
void Renderer::SetUniformV4(uint shaderHandle, const std::string& name, glm::vec4 value) {
    assert(s_Instance != nullptr);
    int modelUniform = glGetUniformLocation(shaderHandle, name.c_str());
    if(modelUniform == -1) {
        if (WarningExists(shaderHandle, name)) return;
        s_Instance->m_Logger.Warning("Couldn't find uniform: " + name);
    }
    glUniform4f(modelUniform, value[0], value[1], value[2], value[3]);
}
void Renderer::SetUniformV3(uint shaderHandle, std::string name, glm::vec3 value) {
    assert(s_Instance != nullptr);
    int modelUniform = glGetUniformLocation(shaderHandle, name.c_str());
    if(modelUniform == -1) {
        if (WarningExists(shaderHandle, name)) return;
        s_Instance->m_Logger.Warning("Couldn't find uniform: " + name);
    }
    glUniform3f(modelUniform, value[0], value[1], value[2]);
}
void Renderer::SetUniformV2(uint shaderHandle, std::string name, glm::vec2 value) {
    assert(s_Instance != nullptr);
    int modelUniform = glGetUniformLocation(shaderHandle, name.c_str());
    if(modelUniform == -1) {
        if (WarningExists(shaderHandle, name)) return;
        s_Instance->m_Logger.Warning("Couldn't find uniform: " + name);
    }
    glUniform2f(modelUniform, value[0], value[1]);
}
void Renderer::SetUniformF(uint shaderHandle, std::string name, float value) {
    assert(s_Instance != nullptr);
    int modelUniform = glGetUniformLocation(shaderHandle, name.c_str());
    if(modelUniform == -1) {
        if (WarningExists(shaderHandle, name)) return;
        s_Instance->m_Logger.Warning("Couldn't find uniform: " + name);
    }
    glUniform1f(modelUniform, value);
}
void Renderer::SetUniformI(uint shaderHandle, std::string name, int value) {
    assert(s_Instance != nullptr);
    int modelUniform = glGetUniformLocation(shaderHandle, name.c_str());
    if(modelUniform == -1) {
        if (WarningExists(shaderHandle, name)) return;
        s_Instance->m_Logger.Warning("Couldn't find uniform: " + name);
    }
    glUniform1i(modelUniform, value);
}

void Renderer::ResizeColorAttachment(int texture, GLenum format, unsigned int width, unsigned int height, GLenum type, GLenum pixelFormat) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, pixelFormat, type, NULL);
}

#pragma once
#include <memory>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <PaperStocking/logger.hpp>
#include <PaperStocking/debug/debugUiManager.hpp>

namespace PaperStocking {
    using GPU_LOC = GLuint;
    struct Renderer {
        public:
            SDL_Window* m_Window;
            SDL_GLContext m_GlContext;

            Renderer();
            ~Renderer();
            void Render();

            // GPU Requests
            static const glm::vec4 GetClearColor();
            static void SetClearColor(glm::vec4 color);
        private:
            static inline Renderer* instance;
            Logger m_Logger = Logger("Renderer", LogColour::Blue);
            glm::vec4 m_ClearColor = glm::vec4(0.8f, 0.5f, 0.7f, 0.8f);
            std::unique_ptr<DebugUiManager> m_DebugUi;

            glm::uvec2 renderResolution = {1280, 720};
            float renderScale = 1.0f;
            glm::uvec2 outputResolution = {1280, 720};

            struct GBuffer {
                // Framebuffer for the GBuffer
                GPU_LOC frameBuffer;

                // Buffer for world-space position of each pixel
                GPU_LOC position;
                // Buffer for world-space normal of each pixel
                GPU_LOC norm;
                // Buffer for scene's lighting
                GPU_LOC lighting;
                // Depth buffer
                GPU_LOC depth;
            } m_GBuffer;
    };
}
#pragma once
#include <memory>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <RmlUi_Renderer_GL3.h>
#include <RmlUi_Platform_SDL.h>

#include <PaperStocking/logger.hpp>
#include <PaperStocking/graphics/camera.hpp>
#include <PaperStocking/debug/debugUiManager.hpp>
#include <PaperStocking/ui/uiManager.hpp>

namespace PaperStocking {
    using GPU_LOC = GLuint;
    struct RenderCommand {
        GPU_LOC shader;
    };
    struct RenderBatch {
        enum BatchSortMode {
            FrontToBack,
            BackToFront
        } m_SortMode;
        std::vector<RenderCommand> m_Commands;
    };
    struct RenderData {
        std::vector<RenderBatch> m_Batches;
    };
    struct CamData {
        glm::mat4 camMat;
        glm::vec3 camPos;
        glm::vec2 screenSize;
    };
    struct Renderer {
        public:
            static inline SDL_Window* s_Window;

            Camera camera;
            SDL_GLContext m_GlContext;

            Renderer();
            ~Renderer();
            void Render();

            // GPU Requests
            static const glm::vec4 GetClearColor();
            static void SetClearColor(glm::vec4 color);

            static void SetOutputResolution(glm::uvec2 resolution, bool setWindow = true);
            static void SetRenderScale(float scale);

            static glm::uvec2 GetOutputResolution();
            static glm::uvec2 GetRenderResolution();
            static float GetRenderScale();
        private:
            static inline Renderer* s_Instance;
            UiManager m_UiManager;
            RenderInterface_GL3* m_UiRenderInterface;
            SystemInterface_SDL* m_UiSystemInterface;
            Logger m_Logger = Logger("Renderer", LogColour::Blue);
            glm::vec4 m_ClearColor = glm::vec4(0.8f, 0.5f, 0.7f, 0.8f);
            std::unique_ptr<DebugUiManager> m_DebugUi;

            glm::uvec2 m_RenderResolution = {1280, 720};
            float m_RenderScale = 1.0f;
            glm::uvec2 m_OutputResolution = {1280, 720};

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

            struct SpriteMeshGPUData {
                uint VAO = 0;
                uint EBO = 0;
            };

            //  UBOs
            GPU_LOC camDataUBO;

            static void UploadFrameData();

            static void EventListener(SDL_Event& event);
    };
}

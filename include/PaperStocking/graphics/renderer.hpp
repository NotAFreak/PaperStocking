#pragma once
#include <memory>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <RmlUi_Renderer_GL3.h>
#include <RmlUi_Platform_SDL.h>

#include <PaperStocking/logger.hpp>
#include <PaperStocking/graphics/renderable/spriteRenderable.hpp>
#include <PaperStocking/ui/uiManager.hpp>
#include <PaperStocking/graphics/camera.hpp>
#include <PaperStocking/debug/debugUiManager.hpp>
#include <PaperStocking/graphics/materialManager.hpp>

#include <PaperStocking/graphics/shader/shaderManager.hpp>

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
            struct SpriteVertex {
                glm::vec3 pos;
                glm::vec2 uv;
            };
            struct SpriteMeshGPUData {
                uint m_VAO = 0;
                uint m_VBO = 0;
                uint m_EBO = 0;
            };
            static inline SpriteMeshGPUData m_QuadData = {0, 0, 0};

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

            static void QueueTextureUpload(std::shared_ptr<Texture> texture);
            static void QueueTextureUnload(std::shared_ptr<Texture> texture);

            static void RegisterSpriteRenderable(std::shared_ptr<SpriteRenderable> sprite);
            static void UnregisterSpriteRenderable(std::shared_ptr<SpriteRenderable> sprite);

            static void SetUniformM4(uint shaderHandle, std::string name, glm::mat4 value);
            static void SetUniformV4(uint shaderHandle, const std::string& name, glm::vec4 value);
            static void SetUniformV3(uint shaderHandle, std::string name, glm::vec3 value);
            static void SetUniformV2(uint shaderHandle, std::string name, glm::vec2 value);
            static void SetUniformF(uint shaderHandle, std::string name, float value);
            static void SetUniformI(uint shaderHandle, std::string name, int value);
        private:
            static inline Renderer* s_Instance;
            std::unique_ptr<DebugUiManager> m_DebugUi;

            UiManager m_UiManager;
            MaterialManager m_MaterialManager;
            ShaderManager m_ShaderManager;
            RenderInterface_GL3* m_UiRenderInterface;
            SystemInterface_SDL* m_UiSystemInterface;
            Logger m_Logger = Logger("Renderer", LogColour::Blue);
            // glm::vec4 m_ClearColor = glm::vec4(0.8f, 0.5f, 0.7f, 0.8f);
            glm::vec4 m_ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

            glm::uvec2 m_RenderResolution = {1280, 720};
            float m_RenderScale = 1.0f;
            glm::uvec2 m_OutputResolution = {1280, 720};

            struct GBuffer {
                std::shared_ptr<ShaderProgram> gBufferShader;
                // Framebuffer for the GBuffer
                GPU_LOC frameBuffer;
                // Framebuffer for the depth prepass
                GPU_LOC depthFrameBuffer;

                // Buffer for color of each pixel
                GPU_LOC color;

                // Buffer for world-space position of each pixel
                GPU_LOC position;
                // Buffer for world-space normal of each pixel
                GPU_LOC norm;
                // Buffer for scene's lighting
                GPU_LOC lighting;
                // Depth buffer
                GPU_LOC depth;
            } m_GBuffer;

            struct CamData {
                glm::mat4 camMat;
                glm::vec3 camPos;
            } m_CamData;

            //  UBOs
            GPU_LOC m_CamDataUBO;

            // Actual render data
            std::vector<std::shared_ptr<SpriteRenderable>> m_Sprites;
            std::mutex m_Sprites_mutex;

            std::queue<std::shared_ptr<Texture>> m_TextureUploadQueue;
            std::mutex m_TextureUploadQueue_mutex;
            std::queue<std::shared_ptr<Texture>> m_TextureUnloadQueue;
            std::mutex m_TextureUnloadQueue_mutex;

            static void UploadFrameData();
            static void EventListener(SDL_Event& event);
            static void ResizeColorAttachment(int texture, GLenum format, unsigned int width, unsigned int height, GLenum type, GLenum pixelFormat);
    };
}

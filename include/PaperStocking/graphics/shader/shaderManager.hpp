#pragma once
#include <map>
#include <thread>
#include <memory>
#include <filesystem>
#include <PaperStocking/logger.hpp>
#include <PaperStocking/graphics/shader/shaderModule.hpp>
#include <PaperStocking/graphics/shader/shaderProgram.hpp>

// Extensions
#ifndef SHADER_FRAGMENT_EXTENSION
#define SHADER_FRAGMENT_EXTENSION ".frag"
#endif
#ifndef SHADER_VERTEX_EXTENSION
#define SHADER_VERTEX_EXTENSION ".vert"
#endif
#ifndef SHADER_COMPUTE_EXTENSION
#define SHADER_COMPUTE_EXTENSION ".comp"
#endif
#ifndef SHADER_TESSELLATION_EXTENSION
#define SHADER_TESSELLATION_EXTENSION ".tess"
#endif
#ifndef SHADER_PROGRAM_EXTENSION
#define SHADER_PROGRAM_EXTENSION ""
#endif

// Asset paths
#ifndef ASSETS_DIRECTORY
#define ASSETS_DIRECTORY "Assets"
#endif
#ifndef SHADER_DIRECTORY
#define SHADER_DIRECTORY "Shaders"
#endif
#ifndef SHADER_PROGRAM_DIRECTORY
#define SHADER_PROGRAM_DIRECTORY "Programs"
#endif
#ifndef SHADER_MODULES_DIRECTORY
#define SHADER_MODULES_DIRECTORY "Modules"
#endif
namespace PaperStocking {
    class GraphicsDevice;
    struct ShaderManager {
        public:
            ShaderManager();           
            ~ShaderManager();

            static void CompileFromQueues();

            static std::shared_ptr<ShaderProgram> GetShaderProgram(std::string name);
            static std::map<std::string, std::weak_ptr<ShaderProgram>>* GetShaderPrograms();
            
            static std::shared_ptr<ShaderModule> GetShaderModule(std::string name);
            static std::map<std::string, std::weak_ptr<ShaderModule>>* GetShaderModules();
        private:
            inline static ShaderManager* s_Instance;
            std::thread::id m_Thread;
            Logger m_Logger = Logger("Shader Manager", LogColour::Blue);

            std::map<std::string, std::weak_ptr<ShaderModule>> m_Modules;
            std::map<std::string, std::weak_ptr<ShaderProgram>> m_Programs;

            std::queue<std::shared_ptr<ShaderModule>> m_ModuleCompileQueue;
            std::mutex m_ModuleCompileQueue_mutex;
            std::queue<std::shared_ptr<ShaderProgram>> m_ProgramCompileQueue;
            std::mutex m_ProgramCompileQueue_mutex;

            static std::shared_ptr<ShaderModule> LoadModule(std::filesystem::path path);
            static std::shared_ptr<ShaderProgram> LoadProgram(std::filesystem::path path);

            static void CompileShaderProgram(std::shared_ptr<ShaderProgram>& program);
            static void CompileShaderModule(std::shared_ptr<ShaderModule>& module);
    };    
}
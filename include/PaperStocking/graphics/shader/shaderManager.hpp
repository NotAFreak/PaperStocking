#pragma once
#include <PaperStocking/logger.hpp>
#include <PaperStocking/graphics/shader/shaderModule.hpp>
#include <PaperStocking/graphics/shader/shaderProgram.hpp>
#include <map>
#include <thread>
#include <filesystem>

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

            static void CompileShaders();
            static void CompileAllModules();
            static void CompileAllPrograms();

            static void CompileShaderProgram(ShaderProgram* program);
            static void CompileShaderModule(ShaderModule* module);

            static ShaderProgram* GetShaderProgram(std::string name);
            static std::map<std::string, ShaderProgram>* GetShaderPrograms();
            
            static ShaderModule* GetShaderModule(std::string name);
            static std::map<std::string, ShaderModule>* GetShaderModules();
        private:
            inline static ShaderManager* s_Instance;
            Logger m_Logger = Logger("Shader Manager", LogColour::Blue);

            std::map<std::string, ShaderModule> m_Modules;
            std::map<std::string, ShaderProgram> m_Programs;

            static void LoadModuleDirectory(std::filesystem::path path);
            static void LoadModule(std::filesystem::path path);
            static void LoadProgramDirectory(std::filesystem::path path);
            static void LoadProgram(std::filesystem::path path);
    };    
}
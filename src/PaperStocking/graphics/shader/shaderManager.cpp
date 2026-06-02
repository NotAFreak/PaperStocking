#include <PaperStocking/graphics/shader/shaderManager.hpp>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <PaperStocking/graphics/renderer.hpp>

using namespace PaperStocking;

ShaderManager::ShaderManager() {
    assert(s_Instance == nullptr);
    s_Instance = this;
}
ShaderManager::~ShaderManager() {
    assert(s_Instance != nullptr);
    s_Instance = nullptr;
}

void ShaderManager::CompileShaders() {
    CompileAllModules();
    CompileAllPrograms();
}

void ShaderManager::LoadModule(std::filesystem::path path) {
    ShaderManager::s_Instance->m_Logger.Info(std::string("Found module: ") + path.string());
    ShaderModule::ModuleType type;
    if(path.extension() == SHADER_VERTEX_EXTENSION) {
        type = ShaderModule::ModuleType::VERTEX;
    } else if(path.extension() == SHADER_FRAGMENT_EXTENSION) {
        type = ShaderModule::ModuleType::FRAGMENT;
    } else if(path.extension() == SHADER_COMPUTE_EXTENSION) {
        type = ShaderModule::ModuleType::COMPUTE;
    } else if(path.extension() == SHADER_TESSELLATION_EXTENSION) {
        type = ShaderModule::ModuleType::TESSELATION;
    } else {
        s_Instance->m_Logger.Error(std::string("Unknown basicLit type: ") + path.string());
    }
    ShaderModule module = ShaderModule(type, path.string());
    CompileShaderModule(&module);

    s_Instance->m_Logger.Info("Compiled module with handle: " + std::to_string(module.handle));
    s_Instance->m_Modules.insert_or_assign(path.filename().string(), module);
}

void ShaderManager::LoadModuleDirectory(std::filesystem::path path) {
    std::filesystem::directory_iterator it(path);
    for (std::filesystem::directory_entry dir : it) {
        if (dir.is_directory()) {
            LoadModuleDirectory(dir.path());
        } else {
            LoadModule(dir.path());
        }
    }
}

void ShaderManager::LoadProgram(std::filesystem::path path) {
    std::vector<std::string> modulePaths;
    {
        std::ifstream inFile = std::ifstream(path);
        std::stringstream stream;
        stream << inFile.rdbuf();
        std::string fileContents = stream.str();

        uint lastIndex = 0;
        for(uint i = 0; i < fileContents.length(); i++) {
            if((fileContents[i] == '\n') || (i == (fileContents.length() - 1))) {
                std::filesystem::path path = fileContents.substr(lastIndex, i);
                modulePaths.push_back(path.filename());
                lastIndex = i+1;
            }
        }
        inFile.close();
    }
    ShaderProgram program = ShaderProgram(path.filename());
    for(std::string modulePath : modulePaths) {
        ShaderModule* mod = &s_Instance->m_Modules[modulePath];
        program.modules.insert_or_assign(mod->type, mod);
    }

    CompileShaderProgram(&program);

    s_Instance->m_Logger.Info("Compiled program with handle: " + std::to_string(program.handle));
    s_Instance->m_Programs.insert_or_assign(path.filename().string(), program);
}

void ShaderManager::LoadProgramDirectory(std::filesystem::path path) {
    std::filesystem::directory_iterator it(path);
    for (std::filesystem::directory_entry dir : it) {
        if (dir.is_directory()) {
            LoadProgramDirectory(dir.path());
        } else {
            LoadProgram(dir.path());
        }
    }
}

void ShaderManager::CompileAllModules() {
    std::filesystem::path moduleDirectory = std::filesystem::path(std::string(ASSETS_DIRECTORY) + "/" + std::string(SHADER_DIRECTORY) + "/" + SHADER_MODULES_DIRECTORY);
    std::filesystem::directory_iterator it(moduleDirectory);

    LoadModuleDirectory(moduleDirectory);
}
void ShaderManager::CompileAllPrograms() {
    std::filesystem::path programDirectory = std::filesystem::path(std::string(ASSETS_DIRECTORY) + "/" + std::string(SHADER_DIRECTORY) + "/" + SHADER_PROGRAM_DIRECTORY);
    std::filesystem::directory_iterator it(programDirectory);

    LoadProgramDirectory(programDirectory);
}

ShaderProgram* ShaderManager::GetShaderProgram(std::string name) {
    return &s_Instance->m_Programs[name];
}
std::map<std::string, ShaderProgram>* ShaderManager::GetShaderPrograms() {
    return &s_Instance->m_Programs;
}

ShaderModule* ShaderManager::GetShaderModule(std::string name) {
    return &s_Instance->m_Modules[name];
}
std::map<std::string, ShaderModule>* ShaderManager::GetShaderModules() {
    return &s_Instance->m_Modules;
}

void ShaderManager::CompileShaderModule(ShaderModule *module) {
    assert(s_Instance != nullptr);
    std::string src;
    std::ifstream inFile;
    inFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    {
        inFile.open(module->location);
        std::stringstream stream;
        stream << inFile.rdbuf();
        inFile.close();
        src = stream.str();
    }
    const char* code = src.c_str();
    uint32_t shaderHandle = 0;
    switch (module->type) {
        case ShaderModule::VERTEX:
            shaderHandle = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderModule::FRAGMENT:
            shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderModule::COMPUTE:
            // shaderHandle = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            throw std::runtime_error("Unknown ShaderModule type: " + module->type);
    }
    glShaderSource(shaderHandle, 1, &code, NULL);

    glCompileShader(shaderHandle);
    {
        int compStatus;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compStatus);
        if(!compStatus) {
            char infoLog[512];
            if(!compStatus) {
                glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
                throw std::runtime_error("Couldn't compile shader: " + std::string(infoLog));
            }
        }
    }
    module->handle = shaderHandle;
}

void ShaderManager::CompileShaderProgram(ShaderProgram *program) {
    assert(s_Instance != nullptr);
    uint32_t programHandle = glCreateProgram();
    uint32_t depthProgramHandle = glCreateProgram();

    for(auto shaderModule : program->modules) {
        glAttachShader(programHandle, shaderModule.second->handle);
        if(shaderModule.first == ShaderModule::ModuleType::VERTEX) {
            glAttachShader(depthProgramHandle, shaderModule.second->handle);
        }
        if(shaderModule.first == ShaderModule::ModuleType::TESSELATION) {
            glAttachShader(depthProgramHandle, shaderModule.second->handle);
        }
    }
    ShaderModule* emptyFrag = ShaderManager::GetShaderModule("empty.frag");
    glAttachShader(depthProgramHandle, emptyFrag->handle);

    {
        int programStatus;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &programStatus);
        if(programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(programHandle, 512, NULL, infoLog);
            s_Instance->m_Logger.Error("Couldn't attach shader modules: " + std::string(infoLog));
        }
    }
    {
        int programStatus;
        glGetProgramiv(depthProgramHandle, GL_LINK_STATUS, &programStatus);
        if(programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(depthProgramHandle, 512, NULL, infoLog);
            s_Instance->m_Logger.Error("Couldn't attach depth prepass shader modules: " + std::string(infoLog));
        }
    }
    glLinkProgram(programHandle);
    {
        int programStatus;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &programStatus);
        if(!programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(programHandle, 512, NULL, infoLog);
            throw std::runtime_error("Couldn't link program modules (Did you forget to add modules to the program?): " + std::string(infoLog));
        }
    }
    glLinkProgram(depthProgramHandle);
    {
        int programStatus;
        glGetProgramiv(depthProgramHandle, GL_LINK_STATUS, &programStatus);
        if(!programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(depthProgramHandle, 512, NULL, infoLog);
            throw std::runtime_error("Couldn't link depth prepass shader modules: " + std::string(infoLog));
        }
    }

    program->handle = programHandle;
    program->depthHandle = depthProgramHandle;
}

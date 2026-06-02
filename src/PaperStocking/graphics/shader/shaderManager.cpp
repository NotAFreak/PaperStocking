#include <cassert>
#include <fstream>
#include <filesystem>
#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/graphics/shader/shaderManager.hpp>

using namespace PaperStocking;

ShaderManager::ShaderManager() {
    assert(s_Instance == nullptr);
    s_Instance = this;
    m_Thread = std::this_thread::get_id();
}
ShaderManager::~ShaderManager() {
    assert(s_Instance != nullptr);
    s_Instance = nullptr;
}

void ShaderManager::CompileFromQueues() {
    assert(s_Instance != nullptr);
    assert(std::this_thread::get_id() == s_Instance->m_Thread);
    {
        std::lock_guard lock(s_Instance->m_ModuleCompileQueue_mutex);
        while (!s_Instance->m_ModuleCompileQueue.empty()) {
            std::shared_ptr<ShaderModule>& module = s_Instance->m_ModuleCompileQueue.front();
            CompileShaderModule(module);
            s_Instance->m_ModuleCompileQueue.pop();
        }
    }
    {
        std::lock_guard lock(s_Instance->m_ProgramCompileQueue_mutex);
        while (!s_Instance->m_ProgramCompileQueue.empty()) {
            std::shared_ptr<ShaderProgram>& program = s_Instance->m_ProgramCompileQueue.front();
            CompileShaderProgram(program);
            s_Instance->m_ProgramCompileQueue.pop();
        }
    }
}

std::shared_ptr<ShaderProgram> ShaderManager::GetShaderProgram(std::string name) {
    if (s_Instance->m_Programs.contains(name)) {
        return s_Instance->m_Programs[name].lock();
    }
    return LoadProgram(std::string(ASSETS_DIRECTORY) + "/" + SHADER_DIRECTORY + "/" + SHADER_PROGRAM_DIRECTORY + "/" + name);
}
std::map<std::string, std::weak_ptr<ShaderProgram>>* ShaderManager::GetShaderPrograms() {
    return &s_Instance->m_Programs;
}

std::shared_ptr<ShaderModule> ShaderManager::GetShaderModule(std::string name) {
    if (s_Instance->m_Modules.contains(name)) {
        if (s_Instance->m_Modules[name].expired()) {
            s_Instance->m_Modules.erase(name);
            return LoadModule(std::string(ASSETS_DIRECTORY) + "/" + SHADER_DIRECTORY + "/" + SHADER_MODULES_DIRECTORY + "/" + name);
        }
        return s_Instance->m_Modules[name].lock();
    }
    return LoadModule(std::string(ASSETS_DIRECTORY) + "/" + SHADER_DIRECTORY + "/" + SHADER_MODULES_DIRECTORY + "/" + name);
}
std::map<std::string, std::weak_ptr<ShaderModule>>* ShaderManager::GetShaderModules() {
    return &s_Instance->m_Modules;
}


std::shared_ptr<ShaderModule> ShaderManager::LoadModule(std::filesystem::path path) {
    assert(s_Instance != nullptr);
    if (s_Instance->m_Modules.contains(path)) {
        return s_Instance->m_Modules[path].lock();
    }
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
        s_Instance->m_Logger.Error(std::string("Unknown shader type: ") + path.string());
    }
    std::shared_ptr<ShaderModule> module = std::make_shared<ShaderModule>(type, path.string());

    if (std::this_thread::get_id() == s_Instance->m_Thread) {
        CompileShaderModule(module);
    } else {
        std::lock_guard lock(s_Instance->m_ModuleCompileQueue_mutex);
        s_Instance->m_ModuleCompileQueue.push(module);
    }

    s_Instance->m_Modules.insert_or_assign(path.filename().string(), std::weak_ptr<ShaderModule>(module));
    return module;
}
std::shared_ptr<ShaderProgram> ShaderManager::LoadProgram(std::filesystem::path path) {
    assert(s_Instance != nullptr);
    if (s_Instance->m_Programs.contains(path)) {
        return s_Instance->m_Programs[path].lock();
    }
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Path not found for shader program: " + path.string());
    }
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
    std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>(path.filename());
    for(std::string modulePath : modulePaths) {
        std::shared_ptr<ShaderModule> module = GetShaderModule(modulePath);
        program->modules.insert_or_assign(module->type, module);
    }

    if (std::this_thread::get_id() == s_Instance->m_Thread) {
        CompileShaderProgram(program);
    } else {
        std::lock_guard lock(s_Instance->m_ProgramCompileQueue_mutex);
        s_Instance->m_ProgramCompileQueue.push(program);
    }

    s_Instance->m_Programs.insert_or_assign(path.filename().string(), std::weak_ptr<ShaderProgram>(program));
    return program;
}
void ShaderManager::CompileShaderModule(std::shared_ptr<ShaderModule>& module) {
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
                throw std::runtime_error("Couldn't compile shader module (" + module->location + "): " + std::string(infoLog));
            }
        }
    }
    module->handle = shaderHandle;
    s_Instance->m_Logger.Info("Compiled module (" + module->location + ") with handle: " + std::to_string(module->handle));
}
void ShaderManager::CompileShaderProgram(std::shared_ptr<ShaderProgram>& program) {
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
    std::shared_ptr<ShaderModule> depthFrag = ShaderManager::GetShaderModule("depth.frag");
    glAttachShader(depthProgramHandle, depthFrag->handle);

    {
        int programStatus;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &programStatus);
        if(programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(programHandle, 512, NULL, infoLog);
            s_Instance->m_Logger.Error("Couldn't attach shader modules (" + program->name + "): " + std::string(infoLog));
        }
    }
    {
        int programStatus;
        glGetProgramiv(depthProgramHandle, GL_LINK_STATUS, &programStatus);
        if(programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(depthProgramHandle, 512, NULL, infoLog);
            s_Instance->m_Logger.Error("Couldn't attach depth prepass shader modules (" + program->name + "): " + std::string(infoLog));
        }
    }
    glLinkProgram(programHandle);
    {
        int programStatus;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &programStatus);
        if(!programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(programHandle, 512, NULL, infoLog);
            throw std::runtime_error("Couldn't link program modules (" + program->name + ") (Did you forget to add modules to the program?): " + std::string(infoLog));
        }
    }
    glLinkProgram(depthProgramHandle);
    {
        int programStatus;
        glGetProgramiv(depthProgramHandle, GL_LINK_STATUS, &programStatus);
        if(!programStatus) {
            char infoLog[512];
            glGetProgramInfoLog(depthProgramHandle, 512, NULL, infoLog);
            throw std::runtime_error("Couldn't link depth prepass shader modules (" + program->name + "): " + std::string(infoLog));
        }
    }

    program->handle = programHandle;
    program->depthHandle = depthProgramHandle;
    s_Instance->m_Logger.Info("Compiled program with handle: " + std::to_string(program->handle));
}

#pragma once
#include <map>
#include <string>
#include "shaderModule.hpp"

namespace PaperStocking {
    struct ShaderProgram {
        public:
            std::string name;
            uint32_t handle = 0U;
            uint32_t depthHandle = 0U;
            std::map<ShaderModule::ModuleType, ShaderModule*> modules;

            // Only existing so compiler doesn't scream at me. Don't use.
            inline ShaderProgram() { }
            ShaderProgram(std::string name);
    };
}
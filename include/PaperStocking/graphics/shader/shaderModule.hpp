#pragma once
#include <string>
#include <cstdint>

namespace PaperStocking {
    struct ShaderModule {
        enum ModuleType : uint8_t {
            VERTEX,
            FRAGMENT,
            COMPUTE,
            TESSELATION
        };
        ModuleType type;
        std::string location;
        uint32_t handle = 0U;

        // Only existing so compiler doesn't scream at me. Don't use.
        inline ShaderModule() { }
        ShaderModule(ModuleType type, std::string location);
    };
}
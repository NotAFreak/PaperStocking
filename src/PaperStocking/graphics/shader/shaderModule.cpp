#include <PaperStocking/graphics/shader/shaderModule.hpp>

using namespace PaperStocking;

ShaderModule::ShaderModule(ModuleType type, std::string location) {
    this->type = type;
    this->location = location;
}
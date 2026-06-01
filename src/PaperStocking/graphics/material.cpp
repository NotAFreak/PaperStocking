#include <PaperStocking/graphics/material.hpp>
#include <PaperStocking/graphics/shader/shaderManager.hpp>

using namespace PaperStocking;

Material::Material(std::string shaderName) {
    this->shaderName = shaderName;
    this->shader = ShaderManager::GetShaderProgram(shaderName);
}
Material::Material(ShaderProgram* shader) {
    this->shader = shader;
    this->shaderName = shader->name;
}
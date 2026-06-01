#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <PaperStocking/graphics/material.hpp>
#include <PaperStocking/graphics/materialManager.hpp>
#include <PaperStocking/graphics/shader/shaderManager.hpp>

using namespace PaperStocking;

MaterialManager::MaterialManager() {
    assert(instance == nullptr);
    instance = this;
}
MaterialManager::~MaterialManager() {
    assert(instance != nullptr);
    instance = nullptr;
}

std::shared_ptr<Material> MaterialManager::LoadMaterial(std::string name) {
    {
        std::lock_guard lock(instance->materials_mutex);
        std::map<std::string, std::weak_ptr<Material>>::iterator it = instance->materials.find(name);
        if (it != instance->materials.end()) {
            return it->second.lock();
        }
    }
    std::filesystem::path directory = std::filesystem::absolute(std::string(ASSETS_DIRECTORY) + "/" + std::string(MATERIAL_DIRECTORY));
    std::filesystem::path path = directory.string() + "/" + name + std::string(MATERIAL_EXTENSION);

    ShaderModule::ModuleType type;
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Material not found!");
    }
    std::string fileData;
    {   //  Read material file
        std::ifstream inFile(path);
        std::stringstream stream;
        stream << inFile.rdbuf();
        fileData = stream.str();
    }
    nlohmann::json j = nlohmann::json::parse(fileData);
    MaterialData matData = j.get<MaterialData>();
    std::shared_ptr<Material> material = std::make_shared<Material>(matData.shader);
    material->uniforms = matData.uniforms;

    std::lock_guard lock(instance->materials_mutex);
    instance->materials.insert_or_assign(path.filename().stem(), std::weak_ptr<Material>(material));
    return material;
}

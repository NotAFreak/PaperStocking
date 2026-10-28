#pragma once
#include <PaperStocking/logger.hpp>
#include <string>
#include <map>
#include <memory>
#include <mutex>

// Extensions
#ifndef MATERIAL_EXTENSION
#define MATERIAL_EXTENSION ".json"
#endif

// Asset paths
#ifndef ASSETS_DIRECTORY
#define ASSETS_DIRECTORY "Assets"
#endif
#ifndef MATERIAL_DIRECTORY
#define MATERIAL_DIRECTORY "Materials"
#endif

namespace PaperStocking {
    struct Material;
    struct MaterialManager {
        public:
            MaterialManager();
            ~MaterialManager();

            static std::shared_ptr<Material> LoadMaterial(std::string name);
        private:
            static inline MaterialManager* instance;
            Logger logger = Logger("Material Manager", LogColour::Blue);
            std::map<std::string, std::weak_ptr<Material>> materials;
            std::mutex materials_mutex;
    };
}
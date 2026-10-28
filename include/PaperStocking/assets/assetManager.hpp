#pragma once
#include <memory>
#include <filesystem>
#include <PaperStocking/assets/metaDb.hpp>
#include <PaperStocking/assets/assetTypes.hpp>

#ifndef ASSETS_FOLDER_NAME
#define ASSETS_FOLDER_NAME_CSTR "Assets"
#define ASSETS_FOLDER_NAME std::string(ASSETS_FOLDER_NAME_CSTR)
#endif

namespace PaperStocking {
    struct AssetManager {
        public:
            AssetManager();
            ~AssetManager();

            static std::shared_ptr<Texture> LoadTexture(TextureLoadRequest request);
            static void LoadTextureAsync(TextureLoadRequest request);
        private:
            static inline AssetManager* s_Instance;
            MetaDB::MetaDatabase database = MetaDB::MetaDatabase(ASSETS_FOLDER_NAME + "/meta.db");
    };
}
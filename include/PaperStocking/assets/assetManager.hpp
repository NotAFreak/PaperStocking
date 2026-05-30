#pragma once
#include <memory>
#include <filesystem>
#include <PaperStocking/assets/assetTypes.hpp>
namespace PaperStocking {
    struct AssetManager {
        public:
            AssetManager();
            ~AssetManager();

            static std::shared_ptr<Texture> LoadTexture(TextureLoadRequest request);
            static void LoadTextureAsync(TextureLoadRequest request);
        private:
            static inline AssetManager* s_Instance;
    };
}
#include <cassert>
#include <PaperStocking/assets/assetManager.hpp>

using namespace PaperStocking;
AssetManager::AssetManager() {
	assert(s_Instance == nullptr);
	s_Instance = this;
}

AssetManager::~AssetManager() {
	assert(s_Instance != nullptr);
	s_Instance = this;
}

std::shared_ptr<Texture> AssetManager::LoadTexture(TextureLoadRequest request) {
	return std::make_shared<Texture>(request.m_AssetPath);
}

void AssetManager::LoadTextureAsync(TextureLoadRequest request) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(request.m_AssetPath);
	request.OnFinishLoad(texture);
}
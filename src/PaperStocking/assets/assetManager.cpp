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

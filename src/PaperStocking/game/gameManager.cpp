#include <PaperStocking/game/scene.hpp>
#include <PaperStocking/game/system.hpp>
#include <PaperStocking/game/gameManager.hpp>

using namespace PaperStocking;

GameManager::GameManager() {
	assert(instance == nullptr);
	s_Instance = this;
}

GameManager::~GameManager() {
	assert(instance != nullptr);
	std::lock_guard sysLock(systems_mutex);
	systems.clear();
	std::lock_guard sceneLock(m_Scenes_mutex);
	m_Scenes.clear();
	s_Instance = nullptr;
}

void GameManager::Update() {
	assert(instance != nullptr);
	{
		std::lock_guard lock(s_Instance->systems_mutex);
		for(const std::shared_ptr<BaseSystem>& sys : s_Instance->systems) {
			assert(sys != nullptr);
			sys->Update();
		}
	} {
		std::lock_guard lock(s_Instance->m_Scenes_mutex);
		for(const std::shared_ptr<Scene>& scene : s_Instance->m_Scenes) {
			assert(scene != nullptr);
			scene->Update();
		}
	}
}

std::weak_ptr<Scene> GameManager::LoadScene() {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_Scenes_mutex);
	std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
	s_Instance->m_Scenes.push_back(newScene);
	return std::weak_ptr<Scene>(newScene);
}

void GameManager::UnloadScene(std::weak_ptr<Scene> scene) {

}
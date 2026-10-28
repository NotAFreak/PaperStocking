#include <PaperStocking/game/scene.hpp>
#include <PaperStocking/game/system.hpp>
#include <PaperStocking/ui/uiManager.hpp>
#include <PaperStocking/game/gameManager.hpp>

using namespace PaperStocking;

GameManager::GameManager() {
	assert(s_Instance == nullptr);
	s_Instance = this;
}

GameManager::~GameManager() {
	assert(s_Instance != nullptr);
	std::lock_guard sysLock(systems_mutex);
	systems.clear();
	std::lock_guard sceneLock(m_Scenes_mutex);
	m_Scenes.clear();
	s_Instance = nullptr;
}

void GameManager::Update() {
	assert(s_Instance != nullptr);
	Timer::Begin("UI Events");
	UiManager::ExecuteAllEvents(UiExecThread::Game);
	Timer::End();
	{
		Timer::Begin("Update Game Systems");
		std::lock_guard lock(s_Instance->systems_mutex);
		for(const std::shared_ptr<BaseSystem>& sys : s_Instance->systems) {
			assert(sys != nullptr);
			Timer::Begin(sys->GetName());
			sys->Update();
			Timer::End();
		}
		Timer::End();
	} {
		Timer::Begin("Update Scenes");
		std::lock_guard lock(s_Instance->m_Scenes_mutex);
		int index = 0;
		for(const std::shared_ptr<Scene>& scene : s_Instance->m_Scenes) {
			assert(scene != nullptr);
			Timer::Begin("Scene [" + std::to_string(index) + "]");
			scene->Update();
			Timer::End();
			index++;
		}
		Timer::End();
	}
	Timer::Begin("Physics");
	m_PhysicsManager.Update();
	Timer::End();
}

std::weak_ptr<Scene> GameManager::LoadScene() {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_Scenes_mutex);
	std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
	s_Instance->m_Scenes.push_back(newScene);
	return std::weak_ptr<Scene>(newScene);
}

void GameManager::UnloadScene(std::weak_ptr<Scene> scene) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_Scenes_mutex);
	std::vector<std::shared_ptr<Scene>>::iterator it;
	std::shared_ptr<Scene> sceneLock = scene.lock();
	for (it = s_Instance->m_Scenes.begin(); it != s_Instance->m_Scenes.end(); ++it) {
		if (it->get() == sceneLock.get()) break;
	}
	assert(it != s_Instance->m_Scenes.end());
	s_Instance->m_Scenes.erase(it);
}

#pragma once
#include <mutex>
#include <vector>
#include <memory>
#include <cassert>

#ifndef GAMEMANAGER_UPDATES_PERSECOND
#define GAMEMANAGER_UPDATES_PERSECOND 100
#endif
namespace PaperStocking {
	struct Scene;
	struct BaseSystem;
	struct GameManager {
		public:
			GameManager();
			~GameManager();

			void Update();
			static std::weak_ptr<Scene> LoadScene();
			static void UnloadScene(std::weak_ptr<Scene> scene);
		private:
			inline static GameManager* s_Instance;

			std::vector<std::shared_ptr<Scene>> m_Scenes;
			std::mutex m_Scenes_mutex;

			std::vector<std::shared_ptr<BaseSystem>> systems;
			std::mutex systems_mutex;
		public:
			// Must be BaseSystem type
			template <class T = BaseSystem>
			static std::weak_ptr<T> AddSystem() {
				assert(s_Instance != nullptr);
				std::lock_guard lock(s_Instance->systems_mutex);
				std::shared_ptr<T> sys = std::make_shared<T>();
				s_Instance->systems.push_back(sys);
				sys->Load();
				return std::weak_ptr<T>(sys);
			}

			template <class T = BaseSystem>
			static void RemoveSystem(std::weak_ptr<T> system) {
				assert(s_Instance != nullptr);
				std::lock_guard lock(s_Instance->systems_mutex);
				std::shared_ptr<T> sysLock = system.lock();
				for (auto it = s_Instance->systems.begin(); it != s_Instance->systems.end(); ++it) {
					if (it->get() == sysLock.get()) {
						s_Instance->systems.erase(it);
						return;
					}
				}
				throw std::runtime_error("Couldn't find system to erase!");
			}
	};
}
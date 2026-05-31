#pragma once
#include <vector>
#include <entt/entt.hpp>
#include <PaperStocking/game/sceneSystem.hpp>
namespace PaperStocking {
	struct Scene final {
		public:
			entt::registry registry;
			Scene() = default;
			~Scene() = default;

			void Update();

			template <class T = BaseSceneSystem>
			std::weak_ptr<T> AddSystem() {
				std::lock_guard lock( m_Systems_mutex);
				std::shared_ptr<T> newSystem = std::make_shared<T>();
				newSystem->m_Scene = this;
				m_Systems.push_back(newSystem);
				newSystem->Load();
				return std::weak_ptr<T>(newSystem);
			}

			template <class T = BaseSceneSystem>
			void RemoveSystem(std::weak_ptr<T> system) {
				std::lock_guard lock(m_Systems_mutex);
				std::shared_ptr<T> sysLock = system.lock();
				for (auto it = m_Systems.begin(); it != m_Systems.end(); ++it) {
					if (it->get() == sysLock.get()) {
						m_Systems.erase(it);
						return;
					}
				}
				throw std::runtime_error("Couldn't find scene system to erase!");
			}
		private:
			std::vector<std::shared_ptr<BaseSceneSystem>> m_Systems;
			std::mutex m_Systems_mutex;
	};
}
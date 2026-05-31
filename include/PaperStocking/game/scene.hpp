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
		private:
			std::vector<std::unique_ptr<BaseSceneSystem>> m_Systems;
			std::mutex m_Systems_mutex;
	};
}
#pragma once
#include <vector>
#include <entt/entt.hpp>
#include <PaperStocking/game/sceneSystem.hpp>
namespace PaperStocking {
	struct Scene final {
		public:
			entt::registry registry;
		private:
			std::vector<std::unique_ptr<BaseSceneSystem>> systems;
	};
}
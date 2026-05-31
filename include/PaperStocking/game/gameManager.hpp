#pragma once
#include <vector>
#include <memory>
#include "scene.hpp"
#include "system.hpp"
namespace PaperStocking {
	struct GameManager {
		public:
			GameManager();
			~GameManager();
			
		private:
			std::vector<Scene> scenes;
			std::vector<std::shared_ptr<BaseSystem>> gameSystems;
	};
}
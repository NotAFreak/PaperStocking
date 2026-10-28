#pragma once
#include "system.hpp"
namespace PaperStocking {
	struct Scene;
	class BaseSceneSystem : public BaseSystem {
		public:
			Scene* m_Scene;

			BaseSceneSystem() = default;
			BaseSceneSystem(Scene* scene);
			~BaseSceneSystem() override = default;
	};
}
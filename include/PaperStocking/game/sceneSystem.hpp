#pragma once
#include "system.hpp"
namespace PaperStocking {
	struct Scene;
	class BaseSceneSystem : public BaseSystem {
		public:
			BaseSceneSystem() = default;
			BaseSceneSystem(Scene* scene);
			~BaseSceneSystem() override = default;
		protected:
			Scene* m_Scene;
	};
}
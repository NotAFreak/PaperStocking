#pragma once
#include "system.hpp"
namespace PaperStocking {
	struct Scene;
	class BaseSceneSystem : public BaseSystem {
		public:
			explicit BaseSceneSystem(Scene* scene);
		protected:
			Scene* m_Scene;
	};
}
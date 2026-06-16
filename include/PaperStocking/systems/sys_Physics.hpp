#pragma once
#include <PaperStocking/game/sceneSystem.hpp>
namespace PaperStocking::Systems {
	class sys_Physics : public BaseSceneSystem {
		public:
			void Load() override;
			void Update() override;
			std::string GetName() override {
				return "sys_Physics";
			}
	};
}
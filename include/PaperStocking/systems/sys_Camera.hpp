#pragma once
#include <PaperStocking/game/sceneSystem.hpp>
namespace PaperStocking::Systems {
	class sys_Camera : public BaseSceneSystem {
		public:
			void Load() override;
			void Update() override;
			std::string GetName() override {
				return "sys_Camera";
			}
	};
}
#pragma once

#include <box2d/box2d.h>
#include <PaperStocking/time/time.hpp>
namespace PaperStocking {
	struct PhysicsManager {
	public:
		b2WorldId world;
		static inline uint8_t s_SubSteps = 2;

		PhysicsManager();
		~PhysicsManager();

		void Update();
	private:
		ThreadTime* time;
	};
}
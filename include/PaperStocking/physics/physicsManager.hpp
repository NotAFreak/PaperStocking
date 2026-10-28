#pragma once
#include <box2d/box2d.h>
#include <PaperStocking/time/time.hpp>
#include <PaperStocking/physics/rigidbody.hpp>
#include <PaperStocking/physics/colliders.hpp>

namespace PaperStocking {
	struct PhysicsManager {
	public:
		static inline uint8_t s_SubSteps = 3;
		static inline float s_TimeScale = 1.0f;
		// i hate this
		static b2WorldId s_World;

		PhysicsManager();
		~PhysicsManager();

		void Update();
	private:
		ThreadTime* time;
	};
}
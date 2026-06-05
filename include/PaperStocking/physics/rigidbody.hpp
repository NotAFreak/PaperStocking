#pragma once
#include <box2d/box2d.h>

#include <PaperStocking/math.hpp>

#include "PaperStocking/game/transform.hpp"

namespace PaperStocking {
	struct StaticRigidbody_CreateInfo {
		Vector2 position = Vector2(0.0f);
		// In Degrees
		float rotation = 0.0f;
	};
	struct StaticRigidbody {
		b2BodyId body;

		StaticRigidbody();
		StaticRigidbody(const StaticRigidbody_CreateInfo& info);
		~StaticRigidbody();
	};
	struct KinematicRigidbody_CreateInfo {
		// Transform* targetTransform = nullptr;
		float linearDampening = 0.0f;
		float angularDampening = 0.0f;

		Vector2 position = Vector2(0.0f);
		Vector2 linearVelocity = Vector2(0.0f);

		// In Degrees
		float rotation = 0.0f;
		float angularVelocity = 0.0f;
	};
	struct KinematicRigidbody {
		b2BodyId body;

		KinematicRigidbody();
		KinematicRigidbody(const KinematicRigidbody_CreateInfo& info);
		~KinematicRigidbody();
	};
	struct DynamicRigidbody_CreateInfo {
		Transform* targetTransform = nullptr;
		float linearDampening = 0.0f;
		float angularDampening = 0.0f;

		Vector2 position = Vector2(0.0f);
		Vector2 linearVelocity = Vector2(0.0f);

		// In Degrees
		float rotation = 0.0f;
		float angularVelocity = 0.0f;
	};
	struct DynamicRigidbody {
		Transform* targetTransform = nullptr;
		b2BodyId body;

		DynamicRigidbody();
		DynamicRigidbody(const DynamicRigidbody_CreateInfo& info);
		~DynamicRigidbody();
	};
}

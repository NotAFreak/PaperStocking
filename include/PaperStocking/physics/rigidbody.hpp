#pragma once
#include <box2d/box2d.h>
#include <PaperStocking/math.hpp>
#include <PaperStocking/game/transform.hpp>

namespace PaperStocking {
	struct StaticRigidbody_CreateInfo {
		Vector2 position = Vector2(0.0f);
		// In Degrees
		float rotation = 0.0f;
	};
	struct StaticRigidbody {
		b2BodyId body = b2_nullBodyId;

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
		b2BodyId body = b2_nullBodyId;
		bool isDestroyed = false;

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
		bool isDestroyed = false;

		DynamicRigidbody();
		DynamicRigidbody(const DynamicRigidbody_CreateInfo& info);
		~DynamicRigidbody();

		void SetPosition(Vector2 pos);
		Vector2 GetPosition();

		void SetLinearVelocity(Vector2 velocity);
		Vector2 GetLinearVelocity();

		void AddLinearForce(Vector2 force);
		void AddLinearForceFromPoint(Vector2 force, Vector2 point);

		float GetMass();
	};
}

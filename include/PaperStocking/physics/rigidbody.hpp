#pragma once
#include <box2d/box2d.h>
#include <PaperStocking/math.hpp>
#include <PaperStocking/game/transform.hpp>

namespace PaperStocking {
	enum RigidbodyLockFlags : uint8_t {
		LOCK_NONE = 1U << 0,
		LOCK_X = 1U << 1,
		LOCK_Y = 1U << 2,
		LOCK_ROT = 1U << 3
	};
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
		Vector2 m_Position = Vector2(0.0f);
		float m_Rotation = 0.0f;
		RigidbodyLockFlags m_LockFlags = LOCK_NONE;
		Transform* m_TargetTransform = nullptr;

		Vector2 m_LinearVelocity = Vector2(0.0f);

		float m_LinearDampening = 0.0f;
		float m_AngularDampening = 0.0f;
		// In Degrees
		float m_AngularVelocity = 0.0f;
		DynamicRigidbody_CreateInfo() = default;
		DynamicRigidbody_CreateInfo(Vector2 position);
		DynamicRigidbody_CreateInfo(Vector2 position, float rotation);
		DynamicRigidbody_CreateInfo(Vector2 position, float rotation, RigidbodyLockFlags);

	};
	struct DynamicRigidbody {
	public:
		Transform* targetTransform = nullptr;
		b2BodyId body;
		bool isDestroyed = false;

		DynamicRigidbody();
		DynamicRigidbody(const DynamicRigidbody_CreateInfo& info);
		~DynamicRigidbody();

		void SetPosition(Vector2 pos);
		Vector2 GetPosition();

		void SetRotation(float degrees);
		// In Degrees
		float GetRotation();

		void SetLinearVelocity(Vector2 velocity);
		void SetLockFlags(RigidbodyLockFlags flags);
		Vector2 GetLinearVelocity();

		void AddLinearForce(Vector2 force);
		void AddLinearForceFromPoint(Vector2 force, Vector2 point);

		float GetMass();
	private:
		RigidbodyLockFlags m_LockFlags;
	};
}

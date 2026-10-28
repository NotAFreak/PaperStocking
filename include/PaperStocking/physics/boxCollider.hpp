#pragma once
#include "baseCollider.hpp"
#include "rigidbody.hpp"

namespace PaperStocking {
	class BoxCollider : public BaseCollider {
		public:
			b2ShapeId shape = b2_nullShapeId;

			BoxCollider(KinematicRigidbody* rigidbody, float size = 1.0f);
			BoxCollider(KinematicRigidbody* rigidbody, float width, float height);

			BoxCollider(DynamicRigidbody* rigidbody, float size = 1.0f);
			BoxCollider(DynamicRigidbody* rigidbody, float width, float height);

			BoxCollider(StaticRigidbody* rigidbody, float size = 1.0f);
			BoxCollider(StaticRigidbody* rigidbody, float width, float height);
			~BoxCollider();
	};
}

#include <PaperStocking/physics/boxCollider.hpp>
#include <PaperStocking/physics/physicsManager.hpp>

using namespace PaperStocking;

// Kinematic Rigidbody
BoxCollider::BoxCollider(KinematicRigidbody *rigidbody, float size) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	float half = size / 2.0f;
	b2Polygon poly = b2MakeBox(half, half);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shape = b2CreatePolygonShape(rigidbody->body, &shapeDef, &poly);
}
BoxCollider::BoxCollider(KinematicRigidbody *rigidbody, float width, float height) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	b2Polygon poly = b2MakeBox(halfWidth, halfHeight);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shape = b2CreatePolygonShape(rigidbody->body, &shapeDef, &poly);
}

// Dynamic Rigidbody
BoxCollider::BoxCollider(DynamicRigidbody *rigidbody, float size) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	b2Polygon poly = b2MakeBox(size, size);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	assert(b2Body_IsValid(rigidbody->body));
	shape = b2CreatePolygonShape(rigidbody->body, &shapeDef, &poly);
}
BoxCollider::BoxCollider(DynamicRigidbody *rigidbody, float width, float height) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	b2Polygon poly = b2MakeBox(width, height);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	assert(b2Body_IsValid(rigidbody->body));
	shape = b2CreatePolygonShape(rigidbody->body, &shapeDef, &poly);
}

// Static Rigidbody
BoxCollider::BoxCollider(StaticRigidbody *rigidbody, float size) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	assert(b2Body_IsValid(rigidbody->body));
}

BoxCollider::BoxCollider(StaticRigidbody *rigidbody, float width, float height) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	assert(b2Body_IsValid(rigidbody->body));
}

BoxCollider::~BoxCollider() {
	assert(b2World_IsValid(PhysicsManager::s_World));
	if (!b2Shape_IsValid(shape)) {
		// hmmm... must have been destroyed by the parented body.
		// totally. yup. Mmhmm.
		return;
	}
	b2DestroyShape(shape, false);
}

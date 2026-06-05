#include <PaperStocking/physics/rigidbody.hpp>

#include "PaperStocking/physics/physicsManager.hpp"

using namespace PaperStocking;

//	Static rigidbody
StaticRigidbody::StaticRigidbody() {

}
StaticRigidbody::StaticRigidbody(const StaticRigidbody_CreateInfo &info) {

}
StaticRigidbody::~StaticRigidbody() {

}

// Kinematic rigidbody
KinematicRigidbody::KinematicRigidbody() {
	b2BodyDef bodyDef = b2BodyDef();
	const KinematicRigidbody_CreateInfo info;
	bodyDef.position = b2Vec2(info.position.x, info.position.y);
	bodyDef.linearDamping = info.linearDampening;
	bodyDef.linearVelocity = b2Vec2(info.linearVelocity.x, info.linearVelocity.y);

	bodyDef.rotation = b2MakeRot(info.rotation * M_PI / 180.0f);
	bodyDef.angularDamping = info.angularDampening;
	bodyDef.angularVelocity = info.angularVelocity;

	b2CreateBody(PhysicsManager::world, &bodyDef);
}
KinematicRigidbody::KinematicRigidbody(const KinematicRigidbody_CreateInfo &info) {
	b2BodyDef bodyDef = b2BodyDef();
	bodyDef.type = b2BodyType::b2_kinematicBody;
	bodyDef.position = b2Vec2(info.position.x, info.position.y);
	bodyDef.linearDamping = info.linearDampening;
	bodyDef.linearVelocity = b2Vec2(info.linearVelocity.x, info.linearVelocity.y);

	bodyDef.rotation = b2Rot(info.rotation);
	bodyDef.angularDamping = info.angularDampening;
	bodyDef.angularVelocity = info.angularVelocity;

	body = b2CreateBody(PhysicsManager::world, &bodyDef);
}
KinematicRigidbody::~KinematicRigidbody() {
	b2DestroyBody(body);
}

// Dynamic rigidbody
DynamicRigidbody::DynamicRigidbody() {
	b2BodyDef bodyDef = b2BodyDef();
	bodyDef.type = b2BodyType::b2_dynamicBody;
	const DynamicRigidbody_CreateInfo info;
	targetTransform = info.targetTransform;
	bodyDef.position = b2Vec2(info.position.x, info.position.y);
	bodyDef.linearDamping = info.linearDampening;
	bodyDef.linearVelocity = b2Vec2(info.linearVelocity.x, info.linearVelocity.y);

	bodyDef.rotation = b2Rot(info.rotation);
	bodyDef.angularDamping = info.angularDampening;
	bodyDef.angularVelocity = info.angularVelocity;

	body = b2CreateBody(PhysicsManager::world, &bodyDef);
}
DynamicRigidbody::DynamicRigidbody(const DynamicRigidbody_CreateInfo &info) {
	b2BodyDef bodyDef = b2BodyDef();
	bodyDef.type = b2BodyType::b2_dynamicBody;
	targetTransform = info.targetTransform;
	bodyDef.position = b2Vec2(info.position.x, info.position.y);
	bodyDef.linearDamping = info.linearDampening;
	bodyDef.linearVelocity = b2Vec2(info.linearVelocity.x, info.linearVelocity.y);

	bodyDef.rotation = b2Rot(info.rotation);
	bodyDef.angularDamping = info.angularDampening;
	bodyDef.angularVelocity = info.angularVelocity;

	body = b2CreateBody(PhysicsManager::world, &bodyDef);
}
DynamicRigidbody::~DynamicRigidbody() {
	b2DestroyBody(body);
}

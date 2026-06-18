#include <PaperStocking/physics/rigidbody.hpp>

#include <PaperStocking/math.hpp>
#include "PaperStocking/logger.hpp"
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
	assert(b2World_IsValid(PhysicsManager::s_World));
	b2BodyDef bodyDef = b2DefaultBodyDef();
	const KinematicRigidbody_CreateInfo info;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = b2Vec2(info.position.x, info.position.y);
	bodyDef.linearDamping = info.linearDampening;
	bodyDef.linearVelocity = b2Vec2(info.linearVelocity.x, info.linearVelocity.y);

	bodyDef.rotation = b2MakeRot(info.rotation * M_PI / 180.0f);
	bodyDef.angularDamping = info.angularDampening;
	bodyDef.angularVelocity = info.angularVelocity;

	body = b2CreateBody(PhysicsManager::s_World, &bodyDef);
}
KinematicRigidbody::KinematicRigidbody(const KinematicRigidbody_CreateInfo &info) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	b2BodyDef bodyDef = b2BodyDef();
	bodyDef.type = b2BodyType::b2_kinematicBody;
	bodyDef.position = b2Vec2(info.position.x, info.position.y);
	bodyDef.linearDamping = info.linearDampening;
	bodyDef.linearVelocity = b2Vec2(info.linearVelocity.x, info.linearVelocity.y);

	bodyDef.rotation = b2MakeRot(info.rotation * M_PI / 180.0f);
	bodyDef.angularDamping = info.angularDampening;
	bodyDef.angularVelocity = info.angularVelocity;

	body = b2CreateBody(PhysicsManager::s_World, &bodyDef);
}
KinematicRigidbody::~KinematicRigidbody() {
	assert(b2World_IsValid(PhysicsManager::s_World));
	assert(b2Body_IsValid(body));
	isDestroyed = true;
	b2DestroyBody(body);
}

// Dynamic rigidbody
DynamicRigidbody_CreateInfo::DynamicRigidbody_CreateInfo(Vector2 position) {
	this->m_Position = position;
}

DynamicRigidbody_CreateInfo::DynamicRigidbody_CreateInfo(Vector2 position, float rotation) {
	this->m_Position = position;
	this->m_Rotation = rotation;
}

DynamicRigidbody_CreateInfo::DynamicRigidbody_CreateInfo(Vector2 position, float rotation, RigidbodyLockFlags flags) {
	this->m_Position = position;
	this->m_Rotation = rotation;
	this->m_LockFlags = flags;
}

DynamicRigidbody::DynamicRigidbody() {
	assert(b2World_IsValid(PhysicsManager::s_World));
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2BodyType::b2_dynamicBody;
	const DynamicRigidbody_CreateInfo info = DynamicRigidbody_CreateInfo();
	targetTransform = info.m_TargetTransform;
	bodyDef.position = b2Vec2(info.m_Position.x, info.m_Position.y);
	bodyDef.linearDamping = info.m_LinearDampening;
	bodyDef.linearVelocity = b2Vec2(info.m_LinearVelocity.x, info.m_LinearVelocity.y);

	bodyDef.rotation = b2MakeRot(info.m_Rotation * M_PI / 180.0f);
	bodyDef.angularDamping = info.m_AngularDampening;
	bodyDef.angularVelocity = info.m_AngularVelocity;

	body = b2CreateBody(PhysicsManager::s_World, &bodyDef);
	assert(b2Body_IsValid(body));
}
DynamicRigidbody::DynamicRigidbody(const DynamicRigidbody_CreateInfo &info) {
	assert(b2World_IsValid(PhysicsManager::s_World));
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2BodyType::b2_dynamicBody;
	targetTransform = info.m_TargetTransform;
	bodyDef.position = b2Vec2(info.m_Position.x, info.m_Position.y);
	bodyDef.linearDamping = info.m_LinearDampening;
	bodyDef.linearVelocity = b2Vec2(info.m_LinearVelocity.x, info.m_LinearVelocity.y);

	bodyDef.rotation = b2MakeRot(info.m_Rotation * DEGTORAD);
	bodyDef.angularDamping = info.m_AngularDampening;
	bodyDef.angularVelocity = info.m_AngularVelocity;
	bodyDef.motionLocks = b2MotionLocks(info.m_LockFlags & LOCK_X, info.m_LockFlags & LOCK_Y, info.m_LockFlags & LOCK_ROT);

	body = b2CreateBody(PhysicsManager::s_World, &bodyDef);
	assert(b2Body_IsValid(body));
}
DynamicRigidbody::~DynamicRigidbody() {
	assert(b2World_IsValid(PhysicsManager::s_World));
	assert(b2Body_IsValid(body));
	isDestroyed = true;
	b2DestroyBody(body);
}

void DynamicRigidbody::SetPosition(Vector2 pos) {
	b2Body_SetTransform(body, (b2Vec2){pos.x, pos.y}, b2Body_GetRotation(body));
}
Vector2 DynamicRigidbody::GetPosition() {
	b2Vec2 pos = b2Body_GetPosition(body);
	return Vector2 {pos.x, pos.y};
}

void DynamicRigidbody::SetRotation(float degrees) {

}

float DynamicRigidbody::GetRotation() {
	return b2Rot_GetAngle(b2Body_GetRotation(body));
}

void DynamicRigidbody::SetLinearVelocity(Vector2 velocity) {
	b2Body_SetLinearVelocity(body, b2Vec2(velocity.x, velocity.y));
}
Vector2 DynamicRigidbody::GetLinearVelocity() {
	b2Vec2 vel = b2Body_GetLinearVelocity(body);
	return Vector2 {vel.x, vel.y};
}

void DynamicRigidbody::AddLinearForce(Vector2 force) {
	b2Body_ApplyForceToCenter(body, b2Vec2{force.x, force.y}, true);
}
void DynamicRigidbody::AddLinearForceFromPoint(Vector2 force, Vector2 point) {
	b2Body_ApplyForce(body, b2Vec2{force.x, force.y}, b2Vec2{point.x, point.y}, true);
}
float DynamicRigidbody::GetMass() {
	return b2Body_GetMass(body);
}

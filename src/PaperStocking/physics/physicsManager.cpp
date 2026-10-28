#include <PaperStocking/physics/physicsManager.hpp>

using namespace PaperStocking;

// what the fuck
b2WorldId PhysicsManager::s_World = b2_nullWorldId;

PhysicsManager::PhysicsManager() {
	time = Time::GetThreadTime(std::this_thread::get_id());
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = {0.0f, 0.0f};
	s_World = b2CreateWorld(&worldDef);
}

PhysicsManager::~PhysicsManager() {
	b2DestroyWorld(s_World);
}

void PhysicsManager::Update() {
	b2World_Step(s_World, time->fDeltaTime * s_TimeScale, s_SubSteps);

	// Apply positions

}

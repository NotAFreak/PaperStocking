#include <PaperStocking/physics/physicsManager.hpp>

using namespace PaperStocking;

PhysicsManager::PhysicsManager() {
	time = Time::GetThreadTime(std::this_thread::get_id());
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = {0.0f, 0.0f};
	world = b2CreateWorld(&worldDef);
}

PhysicsManager::~PhysicsManager() {
	b2DestroyWorld(world);
}

void PhysicsManager::Update() {
	b2World_Step(world, time->fDeltaTime * s_TimeScale, s_SubSteps);

	// Apply positions

}

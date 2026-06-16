#include <PaperStocking/game/scene.hpp>
#include <PaperStocking/systems/sys_Physics.hpp>
#include <PaperStocking/components/cmp_Transform.hpp>
#include <PaperStocking/components/cmp_DynamicRigidbody.hpp>

using namespace PaperStocking;
using namespace PaperStocking::Systems;
using namespace PaperStocking::Components;

void sys_Physics::Load() {

}

void sys_Physics::Update() {
	auto view = m_Scene->registry.view<cmp_Transform, cmp_DynamicRigidbody>();
	for (auto [entity, transform, rigidbody] : view.each()) {
		assert(b2World_IsValid(PhysicsManager::s_World));
		if (!b2Body_IsValid(rigidbody.body)) continue;
		transform.position = Vector3(rigidbody.GetPosition(), transform.position.z);
		transform.rotation = Quaternion(Vector3(0.0f, 0.0f, rigidbody.GetRotation()));
	}
}


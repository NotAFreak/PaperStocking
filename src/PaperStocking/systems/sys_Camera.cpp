
#include <PaperStocking/game/scene.hpp>
#include <PaperStocking/systems/sys_Camera.hpp>
#include <PaperStocking/components/components.hpp>

#include "PaperStocking/graphics/renderer.hpp"

using namespace PaperStocking;
using namespace PaperStocking::Systems;
using namespace PaperStocking::Components;

void sys_Camera::Load() {

}

void sys_Camera::Update() {
	auto view = m_Scene->registry.view<cmp_Transform, cmp_Camera>();
	cmp_Camera* bestCamera = nullptr;
	cmp_Transform* transf = nullptr;
	for (auto [entity, transform, camera] : view.each()) {
		if (bestCamera == nullptr || camera.priority > bestCamera->priority) {
			bestCamera = &camera;
			transf = &transform;
		}
	}
	Camera& cam = Renderer::GetCamera();
	cam.transform.parent = transf;
	if (transf != nullptr) {
		cam.size = bestCamera->size;
		cam.near = bestCamera->near;
		cam.far = bestCamera->far;
	}
}

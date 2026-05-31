#include <PaperStocking/game/scene.hpp>

using namespace PaperStocking;

void Scene::Update() {
	std::lock_guard lock(m_Systems_mutex);
	for (const auto& system : m_Systems) {
		system->Update();
	}
}

#include <PaperStocking/game/sceneSystem.hpp>
#include <PaperStocking/game/scene.hpp>
using namespace PaperStocking;

BaseSceneSystem::BaseSceneSystem(Scene* scene) {
	this->m_Scene = scene;
}

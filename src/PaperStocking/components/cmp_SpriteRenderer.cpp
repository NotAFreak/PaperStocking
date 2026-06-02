#include <PaperStocking/components/cmp_SpriteRenderer.hpp>

#include <PaperStocking/graphics/renderer.hpp>

using namespace PaperStocking;
using namespace PaperStocking::Components;

cmp_SpriteRenderer::cmp_SpriteRenderer(std::string materialName) {
	m_Sprite = std::make_shared<SpriteRenderable>(materialName);
	Renderer::RegisterSpriteRenderable(m_Sprite);
}

cmp_SpriteRenderer::cmp_SpriteRenderer(Transform* parent, std::string materialName) {
	m_Sprite = std::make_shared<SpriteRenderable>(materialName);
	m_Sprite->m_Transform.parent = parent;
	Renderer::RegisterSpriteRenderable(m_Sprite);
}

cmp_SpriteRenderer::~cmp_SpriteRenderer() {
	Renderer::UnregisterSpriteRenderable(m_Sprite);
}

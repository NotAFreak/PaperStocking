#include <PaperStocking/components/cmp_SpriteRenderer.hpp>

using namespace PaperStocking;
using namespace PaperStocking::Components;

cmp_SpriteRenderer::cmp_SpriteRenderer(std::string materialName) {
	m_Sprite = std::make_unique<SpriteRenderable>(materialName);
}

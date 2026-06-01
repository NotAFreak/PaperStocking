#pragma once
#include <PaperStocking/graphics/renderable/spriteRenderable.hpp>
namespace PaperStocking::Components {
	struct cmp_SpriteRenderer {
		public:
			std::unique_ptr<SpriteRenderable> m_Sprite;
			cmp_SpriteRenderer(std::string materialName);
			~cmp_SpriteRenderer() = default;
	};
}
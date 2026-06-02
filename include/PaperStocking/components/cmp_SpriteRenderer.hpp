#pragma once
#include <PaperStocking/graphics/renderable/spriteRenderable.hpp>
namespace PaperStocking::Components {
	struct cmp_SpriteRenderer {
		public:
			std::shared_ptr<SpriteRenderable> m_Sprite;
			cmp_SpriteRenderer(std::string materialName);
			cmp_SpriteRenderer(Transform* parent, std::string materialName);
			~cmp_SpriteRenderer();
	};
}
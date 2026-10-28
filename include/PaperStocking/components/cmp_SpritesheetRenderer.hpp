#pragma once
#include <PaperStocking/graphics/renderable/spritesheetRenderable.hpp>
namespace PaperStocking::Components {
	struct cmp_SpritesheetRenderer {
	public:
		std::shared_ptr<SpritesheetRenderable> m_Sprite;
		cmp_SpritesheetRenderer(std::string materialName);
		cmp_SpritesheetRenderer(Transform* parent, std::string materialName);
		~cmp_SpritesheetRenderer();
	};
}
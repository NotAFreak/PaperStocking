#pragma once
#include <memory>
#include <PaperStocking/game/transform.hpp>
#include <PaperStocking/graphics/baseRenderable.hpp>

namespace PaperStocking {
	struct Texture;
	struct Material;
	// Only create on render thread
	class SpriteRenderable final : BaseRenderable {
		public:
			Transform m_Transform;
			std::shared_ptr<Material> m_Material;

			SpriteRenderable(std::string materialName);
			~SpriteRenderable();
			void Render() override;
	};
}

#pragma once
#include <memory>
#include <PaperStocking/game/transform.hpp>
#include <PaperStocking/graphics/baseRenderable.hpp>

#include "PaperStocking/math.hpp"

namespace PaperStocking {
	using SpriteIndex = uint8_t;
	struct Texture;
	struct Material;
	enum SpritesheetDirection : uint8_t {
		DIR_RIGHTDOWN,
		DIR_RIGHTUP,
		DIR_LEFTDOWN,
		DIR_LEFTUP,
		DIR_DOWNRIGHT,
		DIR_UPRIGHT,
		DIR_DOWNLEFT,
		DIR_UPLEFT
	};
	// Only create on render thread
	class SpritesheetRenderable final : BaseRenderable {
	public:
		Transform m_Transform;
		std::shared_ptr<Material> m_Material;

		uint16_t m_CellWidth = 32;
		uint16_t m_CellHeight = 32;

		uint16_t m_TextureWidth = 512;
		uint16_t m_TextureHeight = 512;

		SpritesheetDirection m_Direction = DIR_RIGHTDOWN;
		SpriteIndex m_StartCell = 0;
		SpriteIndex m_CellCount = -1U;

		SpriteIndex m_CurrentIndex = 0;

		Vector2 GenerateUVs(SpriteIndex index, Vector2 originalUV);

		SpritesheetRenderable(std::string materialName);
		~SpritesheetRenderable();
		void Render() override;
	};
}

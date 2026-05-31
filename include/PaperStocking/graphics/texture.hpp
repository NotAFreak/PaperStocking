#pragma once
#include <memory>
#include <filesystem>

namespace PaperStocking {
	enum TextureWrap {
		REPEAT = 0,
		CLAMP_EDGE = 1,
		CLAMP_BORDER = 2
	};
	enum TextureFilter {
		POINT = 0,
		BILINEAR = 1,
		TRILINEAR = 2
	};
	enum TextureFormat : uint8_t {
		R8 = 0,
		RG8 = 1,
		RGB8 = 2,
		RGBA8 = 3,
		SRGB8 = 4,
		SRGB8_ALPHA8 = 5,
		R16F = 6,
		RG16F = 7,
		RGB16F = 8,
		RGBA16F = 9,
		R32F = 10,
		RG32F = 11,
		RGB32F = 12,
		RGBA32F = 13,
		DEPTH_COMPONENT16 = 14,
		DEPTH_COMPONENT24 = 15,
		DEPTH_COMPONENT32F = 16,
		DEPTH24_STENCIL8 = 17,
		DEPTH32F_STENCIL8 = 18
	};
	struct Texture {
		public:
			static std::shared_ptr<Texture> LoadTexture(std::istream in);
			static std::shared_ptr<Texture> LoadTexture(std::filesystem::path filePath);
		private:
			uint16_t width = 64;
			uint16_t height = 64;
			TextureWrap m_Wrap = TextureWrap::CLAMP_EDGE;
			TextureFilter m_MinFilter = TextureFilter::POINT;
			TextureFilter m_MagFilter = TextureFilter::POINT;
			TextureFormat m_Format = TextureFormat::RGB8;
	};
}

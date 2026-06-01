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
			Texture() = default;
			Texture(std::filesystem::path filePath);

			~Texture();
			bool IsUploaded() const {
				return m_Uploaded;
			}

			// Only on render thread
			void Upload();
			// Only on render thread
			void Unload();

			// Only on render thread
			void Update();
			// Only on render thread
			void UpdateSafe();

			uint32_t GetHandle() const {
				return m_Handle;
			}
			uint8_t* GetPixels() const {
				return m_Pixels;
			}
			uint16_t GetWidth() const {
				return m_Width;
			}
			uint16_t GetHeight() const {
				return m_Height;
			}

			std::string m_Location;
			TextureWrap m_Wrap = TextureWrap::CLAMP_EDGE;
			TextureFilter m_MinFilter = TextureFilter::POINT;
			TextureFilter m_MagFilter = TextureFilter::POINT;
			TextureFormat m_Format = TextureFormat::RGB8;
			bool m_EnableMipmaps = false;
		private:
			uint32_t m_Handle = 0x0;
			bool m_Uploaded = false;

			uint16_t m_Width = 64;
			uint16_t m_Height = 64;
			uint8_t* m_Pixels = nullptr;
	};
}

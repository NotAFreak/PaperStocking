#include <fstream>
#include <stb_image.h>
#include <stb_image_impl.h>
#include <PaperStocking/assets/metaDb.hpp>
#include <PaperStocking/graphics/texture.hpp>
#include <PaperStocking/graphics/renderer.hpp>

using namespace PaperStocking;

Texture::Texture(std::filesystem::path filePath) {
    uint8_t* data;
    int w, h;
    int channels;
    data = stbi_load(filePath.c_str(), &w, &h, &channels, STBI_rgb_alpha);
    if(!data) {
        throw std::runtime_error("Failed to load image data from path: " + filePath.string());
    }
    m_Pixels = data;
    m_Width = w;
    m_Height = h;
    if (MetaDB::MetaDatabase::HasTexture(filePath)) {
        MetaDB::T_Texture metadata = MetaDB::MetaDatabase::GetTexture(filePath);
        m_Format = metadata.format;
        m_MagFilter = metadata.magFilter;
        m_MinFilter = metadata.minFilter;
        m_Wrap = metadata.wrap;
    }
}

Texture::~Texture() {
    if (m_Pixels != nullptr) {
        delete[] m_Pixels;
    }
}

void Texture::Upload() {
	assert(!IsUploaded());
	glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);
    GLint wrap;
    switch (m_Wrap){
        case TextureWrap::REPEAT:
            wrap = GL_REPEAT;
            break;
        case TextureWrap::CLAMP_EDGE:
            wrap = GL_CLAMP_TO_EDGE;
            break;
        case TextureWrap::CLAMP_BORDER:
            wrap = GL_CLAMP_TO_BORDER;
        default:
            throw std::runtime_error("Unknown wrap mode: " + std::to_string(m_Wrap));
    }
    GLint min;
    switch (m_MinFilter){
        case TextureFilter::POINT:
            min = GL_NEAREST;
            break;
        case TextureFilter::BILINEAR:
            min = GL_LINEAR;
            break;
        case TextureFilter::TRILINEAR:
            min = GL_LINEAR_MIPMAP_LINEAR;
            break;
        default:
            throw std::runtime_error("Unknown filter mode: " + std::to_string(m_MinFilter));
    }
    GLint mag;
    switch (m_MagFilter){
        case TextureFilter::POINT:
            mag = GL_NEAREST;
            break;
        case TextureFilter::BILINEAR:
            mag = GL_LINEAR;
            break;
        case TextureFilter::TRILINEAR:
            mag = GL_LINEAR_MIPMAP_LINEAR;
            break;
        default:
            throw std::runtime_error("Unknown filter mode: " + std::to_string(m_MagFilter));
    }
    GLint internalFormat = GL_RGBA;
    GLint pixelFormat = GL_RGBA;
    switch (m_Format) {
        case R8:
            internalFormat = GL_R8;
            break;
        case RG8:
            internalFormat = GL_RG8;
            break;
        case RGB8:
            internalFormat = GL_RGB8;
            break;
        case RGBA8:
            internalFormat = GL_RGBA8;
            break;
        case SRGB8:
            internalFormat = GL_SRGB8;
            break;
        case SRGB8_ALPHA8:
            internalFormat = GL_SRGB8_ALPHA8;
            break;
        case R16F:
            internalFormat = GL_R16F;
            break;
        case RG16F:
            internalFormat = GL_RG16F;
            break;
        case RGB16F:
            internalFormat = GL_RGB16F;
            break;
        case RGBA16F:
            internalFormat = GL_RGBA16F;
            break;
        case R32F:
            internalFormat = GL_R32F;
            break;
        case RG32F:
            internalFormat = GL_RG32F;
            break;
        case RGB32F:
            internalFormat = GL_RGB32F;
            break;
        case RGBA32F:
            internalFormat = GL_RGBA32F;
            break;
        case DEPTH_COMPONENT16:
            internalFormat = GL_DEPTH_COMPONENT16;
            break;
        case DEPTH_COMPONENT24:
            internalFormat = GL_DEPTH_COMPONENT24;
            break;
        case DEPTH_COMPONENT32F:
            internalFormat = GL_DEPTH_COMPONENT32;
            break;
        case DEPTH24_STENCIL8:
            internalFormat = GL_DEPTH24_STENCIL8;
            break;
        case DEPTH32F_STENCIL8:
            internalFormat = GL_DEPTH32F_STENCIL8;
            break;
        default:
            throw std::runtime_error("Unknown texture format: " + m_Format);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, pixelFormat, GL_UNSIGNED_BYTE, m_Pixels);
    if (m_EnableMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    m_Uploaded = true;
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unload() {

}

void Texture::Update() {
	assert(IsUploaded());
}
void Texture::UpdateSafe() {
	if (!IsUploaded()) {
		Upload();
	} else {
		Update();
	}
}
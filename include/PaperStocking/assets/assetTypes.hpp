#pragma once
#include <PaperStocking/assets/assetManager.hpp>

#include <PaperStocking/graphics/texture.hpp>
#include <PaperStocking/ui/uiDocument.hpp>

namespace PaperStocking {
	template <typename AssetType>
	class BaseAssetLoadRequest {
		public:
			std::filesystem::path m_AssetPath;
			void(*OnFinishLoad)(std::shared_ptr<AssetType>);
	};

	class TextureLoadRequest : public BaseAssetLoadRequest<Texture> { };
	class UiLoadRequest : public BaseAssetLoadRequest<UiDocument> {};
}
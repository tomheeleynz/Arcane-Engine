#include "TextureAsset.h"

namespace Arcane
{
	TextureAsset::TextureAsset(std::filesystem::path filepath)
	{
		SetAssetType(AssetType::TEXTURE);
		m_Texture = Texture::Create(filepath.string());
	}

	Texture* TextureAsset::GetTexture()
	{
		return m_Texture;
	}
}
#include "TextureAsset.h"

namespace Arcane
{
	TextureAsset::TextureAsset(std::filesystem::path filepath)
	{
		SetAssetType(AssetType::TEXTURE);
		m_TexturePath = filepath;
	}

	Texture* TextureAsset::GetTexture()
	{
		return m_Texture;
	}

	void TextureAsset::LoadAsset()
	{
		m_Texture = Texture::Create(m_TexturePath.string());
	}
}
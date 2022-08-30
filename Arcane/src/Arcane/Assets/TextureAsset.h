#pragma once

#include <filesystem>
#include "Asset.h"
#include "Arcane/Renderer/Texture.h"

namespace Arcane
{
	class TextureAsset : public Asset
	{
	public:
		TextureAsset(std::filesystem::path filepath);

		Texture* GetTexture();

		virtual void LoadAsset() override;
	private:
		Texture* m_Texture;
		std::filesystem::path m_TexturePath;
	};
}
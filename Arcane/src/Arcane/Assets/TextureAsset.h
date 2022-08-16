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
	private:
		Texture* m_Texture;
	};
}
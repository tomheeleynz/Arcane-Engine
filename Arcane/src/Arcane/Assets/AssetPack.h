#pragma once

#include <filesystem>
#include <fstream>

#include "Arcane/Core/Application.h"

namespace Arcane
{
	struct AssetPackTexture
	{
		uint64_t id;
		uint32_t width;
		uint32_t height;
		uint32_t textureDataCount;
		std::vector<unsigned char> textureData;
	};

	struct AssetPackHeader
	{
		uint32_t signature;
		uint8_t version;
	};

	struct AssetPackFile
	{
		AssetPackHeader assetPackHeader;
		uint16_t textureCount;
		std::vector<AssetPackTexture> textures;
	};

	class AssetPack
	{
	public:
		AssetPack(std::filesystem::path path);

		void Serialize();
		void Deserialize();
	private:
		std::filesystem::path m_Path;
	};
}
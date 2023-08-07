#include "AssetPack.h"

namespace Arcane
{
	AssetPack::AssetPack(std::filesystem::path path)
	{
		m_Path = path;
	}

	void AssetPack::Serialize()
	{
		// Get Asset Database
		AssetDatabase& database = Application::Get().GetAssetDatabase();
		
		// Asset Pack File Struct
		AssetPackFile newFile;

		// Create header for the struct 
		AssetPackHeader newHeader;
		newHeader.signature = 115;
		newHeader.version = 0x01;

		// Write header to file
		std::ofstream o(m_Path, std::ios::out | std::ios::binary);

		uint16_t textureCount = 0;
		std::vector<AssetPackTexture> assetPackTextures;

		for (const auto& [key, val] : database.GetAssetMap())
		{
			if (val->GetAssetType() == AssetType::TEXTURE) {
				textureCount += 1;
			}
		}

		int savedTextures = 0;
		newFile.textures.resize(textureCount);
		for (const auto& [key, val] : database.GetAssetMap())
		{
			if (val->GetAssetType() == AssetType::TEXTURE)
			{
				Texture* texture = static_cast<Texture*>(val);

				newFile.textures[savedTextures].height = texture->GetTextureSpecs().height;
				newFile.textures[savedTextures].width = texture->GetTextureSpecs().width;
				newFile.textures[savedTextures].id = key;
				
				TextureRuntimeData runtimeData = texture->GetRuntimeData();
				newFile.textures[savedTextures].textureDataCount = runtimeData.size;

				for (int i = 0; i < runtimeData.size; i++) {
					newFile.textures[savedTextures].textureData.push_back(runtimeData.data[i]);
				}
				
				savedTextures++;
			}
		}

		newFile.assetPackHeader = newHeader;
		newFile.textureCount = textureCount;

		o.write((char*)&newHeader, sizeof(AssetPackHeader));
		o.write((char*)&textureCount, sizeof(uint16_t));
		o.write((char*)&newFile.textures[0], sizeof(assetPackTextures) * textureCount);

		o.close();
	}

	void AssetPack::Deserialize()
	{
		std::ifstream i(m_Path, std::ios::out | std::ios::binary);

		AssetPackHeader newHeader;
		uint16_t textureCount;
		std::vector<AssetPackTexture> textures;

		// Get Header
		i.read((char*)&newHeader, sizeof(AssetPackHeader));
		
		// Get Texture Count
		i.read((char*)&textureCount, sizeof(uint16_t));
		
		// Get Textures
		textures.resize(textureCount);
		i.read((char*)&textures[0], textureCount * sizeof(AssetPackTexture));


		i.close();
	}
}
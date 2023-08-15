#include <vector>

#include "AssetPack.h"
#include "Arcane/Scene/SceneManager.h"
#include "Arcane/ECS/Entity.h"
#include "Arcane/Scene/Scene.h"
#include "Arcane/Scene/SceneDeserializer.h"

namespace Arcane
{
	AssetPack::AssetPack(std::filesystem::path path)
	{
		m_Path = path;
	}

	void AssetPack::Serialize()
	{
		// Serialize ScenesPack
		std::ofstream o(m_Path, std::ios::out | std::ios::binary);
		AssetDatabase& database = Application::Get().GetAssetDatabase();

		nlohmann::json sceneJson;
		sceneJson["Scenes"] = nlohmann::json::array();

		for (auto& [id, asset] : database.GetAssetMap())
		{
			if (asset->GetAssetType() == AssetType::SCENE)
			{
				nlohmann::json jsonObject;
				std::ifstream i(asset->GetPath());
				i >> jsonObject;
				
				sceneJson["Scenes"].push_back(jsonObject);
			}
		}

		std::string jsonString = sceneJson.dump();
		uint32_t jsonLength = jsonString.length();

		o.write((const char*)&jsonLength, sizeof(uint32_t));
		o.write(jsonString.data(), jsonLength);

		for (auto& [id, asset] : database.GetAssetMap())
		{
			if (asset->GetAssetType() == AssetType::TEXTURE)
			{
				asset->PackAsset(o);
				break;
			}
		}


		o.close();
	}

	void AssetPack::Deserialize()
	{
		std::ifstream i(m_Path, std::ios::out | std::ios::binary);
		AssetDatabase& database = Application::Get().GetAssetDatabase();

		uint32_t sceneJsonStringLength = 0;
		i.read((char*)&sceneJsonStringLength, sizeof(uint32_t));

		std::string sceneJsonString;
		sceneJsonString.resize(sceneJsonStringLength);
		i.read((char*)sceneJsonString.data(), sceneJsonStringLength);

		SceneDeserializer deserializer = SceneDeserializer();
		nlohmann::json scenesJson = nlohmann::json::parse(sceneJsonString);

		for (auto& element : scenesJson["Scenes"])
		{
			deserializer.Deserialize(element);
		}

		while (true)
		{
			if (i.eof()) break;

			int type = 0;
			i.read((char*)&type, sizeof(int));

			if ((AssetType)type == AssetType::TEXTURE)
			{
				std::pair<uint64_t, Asset*> unpackedAsset = UnpackAsset(AssetType::TEXTURE, i);
				database.AddAsset(unpackedAsset.first, unpackedAsset.second);
			}

		}


		i.close();
	}

	std::pair<uint64_t, Asset*> AssetPack::UnpackAsset(AssetType type, std::ifstream& i)
	{
		std::pair<uint64_t, Asset*> newAsset = std::pair<uint64_t, Asset*>();

		if (type == AssetType::TEXTURE)
		{
			uint64_t id = 0;
			i.read((char*)&id, sizeof(uint64_t));

			newAsset.first = id;
			newAsset.second = UnpackTexture(i);
		}

		return newAsset;
	}


	Asset* AssetPack::UnpackTexture(std::ifstream& i)
	{
		uint32_t totalLength = 0;
		i.read((char*)&totalLength, sizeof(uint32_t));

		uint32_t jsonLength = 0;
		i.read((char*)&jsonLength, sizeof(uint32_t));

		uint32_t blobLength = 0;
		i.read((char*)&blobLength, sizeof(uint32_t));

		// Get Texture Metadata
		std::string jsonString;
		jsonString.resize(jsonLength);
		i.read((char*)jsonString.data(), jsonLength);
		nlohmann::json textureMetadata = nlohmann::json::parse(jsonString);

		// Get Binary Data
		std::vector<char> blob;
		blob.resize(blobLength);
		i.read((char*)blob.data(), blobLength);

		return Texture::Create(blob.data(), blob.size(), textureMetadata["width"], textureMetadata["height"]);
	}
}
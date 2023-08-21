#include <vector>

#include "AssetPack.h"
#include "Arcane/Scene/SceneManager.h"
#include "Arcane/ECS/Entity.h"
#include "Arcane/Scene/Scene.h"
#include "Arcane/Scene/SceneDeserializer.h"

namespace Arcane
{
	AssetPack::AssetPack(AssetDatabase* database)
	{
		m_AssetDatabase = database;
	}

	void AssetPack::Serialize(std::filesystem::path path)
	{
		// Serialize ScenesPack
		std::ofstream o(path, std::ios::out | std::ios::binary);

		nlohmann::json sceneJson;
		sceneJson["Scenes"] = nlohmann::json::array();

		for (auto& [id, asset] : m_AssetDatabase->GetAssetMap())
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

		int count = 0;
		for (auto& [id, asset] : m_AssetDatabase->GetAssetMap())
		{
			if (asset->GetAssetType() == AssetType::SCRIPT)
			{
				asset->PackAsset(o);
				count += 1;
			}

			if (asset->GetAssetType() == AssetType::TEXTURE)
			{
				asset->PackAsset(o);
				count += 1;
			}

			if (asset->GetAssetType() == AssetType::SHADER)
			{
				asset->PackAsset(o);
				count += 1;
			}

			if (count == 3)
				break;
		}

		// Pack the shader library for engine wide shaders
		ShaderLibrary::PackShaderLibrary(o);

		o.close();
	}

	void AssetPack::Deserialize(std::filesystem::path path)
	{
		std::ifstream i(path, std::ios::out | std::ios::binary);

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
				m_AssetDatabase->AddAsset(unpackedAsset.first, unpackedAsset.second);
			}

			if ((AssetType)type == AssetType::SCRIPT)
			{
				std::pair<uint64_t, Asset*> unpackedAsset = UnpackAsset(AssetType::SCRIPT, i);
				m_AssetDatabase->AddAsset(unpackedAsset.first, unpackedAsset.second);
			}


			if ((AssetType)type == AssetType::SHADER)
			{
				std::pair<uint64_t, Asset*> unpackedAsset = UnpackAsset(AssetType::SHADER, i);
				m_AssetDatabase->AddAsset(unpackedAsset.first, unpackedAsset.second);
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


		if (type == AssetType::SCRIPT)
		{
			uint64_t id = 0;
			i.read((char*)&id, sizeof(uint64_t));

			newAsset.first = id;
			newAsset.second = UnpackScript(i);
		}

		if (type == AssetType::SHADER)
		{
			uint64_t id = 0;
			i.read((char*)&id, sizeof(uint64_t));
			newAsset.first = id;
			newAsset.second = UnpackShader(i);
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

	Asset* AssetPack::UnpackScript(std::ifstream& i)
	{
		uint32_t totalLength = 0;
		i.read((char*)&totalLength, sizeof(uint32_t));

		uint32_t byteCodeLength = 0;
		i.read((char*)&byteCodeLength, sizeof(uint32_t));

		std::vector<char> byteCode;
		byteCode.resize(byteCodeLength);
		i.read((char*)byteCode.data(), byteCodeLength);

		return nullptr;
	}

	Asset* AssetPack::UnpackShader(std::ifstream& i)
	{
		uint32_t totalLength = 0;
		i.read((char*)&totalLength, sizeof(uint32_t));
		
		// Get Vertex Bytes
		uint32_t vertexBytesSize = 0;
		i.read((char*)&vertexBytesSize, sizeof(uint32_t));

		std::vector<uint32_t> vertexBytes;
		vertexBytes.resize(vertexBytesSize);
		i.read((char*)vertexBytes.data(), vertexBytesSize * sizeof(uint32_t));

		// Get Fragment Bytes
		uint32_t fragmentBytesSize = 0;
		i.read((char*)&fragmentBytesSize, sizeof(uint32_t));

		std::vector<uint32_t> fragmentBytes;
		fragmentBytes.resize(fragmentBytesSize);
		i.read((char*)fragmentBytes.data(), fragmentBytesSize * sizeof(uint32_t));

		return Shader::Create(vertexBytes.data(), vertexBytesSize, fragmentBytes.data(), fragmentBytesSize);
	}
}
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <time.h>
#include <cstdlib>

#include "AssetDatabase.h"
#include "Arcane/Renderer/Shader.h"
#include "Arcane/Core/UUID.h"
#include "Arcane/Renderer/Mesh.h"
#include "Arcane/Renderer/Texture.h"
#include "Arcane/Scripting/Script.h"

namespace Arcane
{
	AssetDatabase::AssetDatabase(std::filesystem::path assetDir)
	{
		m_AssetDirPath = assetDir;
	}

	Asset* AssetDatabase::GetAsset(uint64_t id)
	{
		return m_Assets[id];
	}

	Asset* AssetDatabase::GetDefaultAsset(std::string name)
	{
		return GetAsset(m_DefaultAssets[name]);
	}

	bool AssetDatabase::GenerateDatabase()
	{
		GenerateDefaultAssets();
		for (auto& dirEntry : std::filesystem::recursive_directory_iterator(m_AssetDirPath)) {
			if (dirEntry.is_regular_file()) {
				std::filesystem::path file = dirEntry.path();

				bool isAssetGenerated = GenerateAsset(file);
				if (!isAssetGenerated) {
					return false;
				}
			}
		}
		return true;
	}

	bool AssetDatabase::GenerateAsset(std::filesystem::path currentAssetPath)
	{
		std::string metaFileName = currentAssetPath.stem().string() + ".arcmeta";
		std::filesystem::path metaPath = currentAssetPath.parent_path() / metaFileName;

		// Either Create asset id or get from the meta file
		uint64_t assetID = 0;
		if (!CheckMetaInfo(metaPath)) {
			assetID = (uint64_t)Arcane::Core::UUID();
			GenerateMetaFile(metaPath, assetID);
		}
		else {
			assetID = GetMetaID(metaPath);
		}

		// Get File extension
		if (currentAssetPath.extension() == ".obj") {
			Mesh* newMeshAsset = new Mesh(currentAssetPath.string());
			newMeshAsset->SetID(Arcane::Core::UUID(assetID));
			newMeshAsset->SetAssetType(AssetType::MESH);
			m_Assets[assetID] = newMeshAsset;
		}
		else if (currentAssetPath.extension() == ".fbx") {
			Mesh* newMeshAsset = new Mesh(currentAssetPath.string());
			newMeshAsset->SetAssetType(AssetType::MESH);
			newMeshAsset->SetID(Arcane::Core::UUID(assetID));
			m_Assets[assetID] = newMeshAsset;
		}
		else if (currentAssetPath.extension() == ".jpg") {
			Texture* newTextureAsset = Texture::Create(currentAssetPath.string());
			newTextureAsset->SetAssetType(AssetType::TEXTURE);
			newTextureAsset->SetID(Arcane::Core::UUID(assetID));
			m_Assets[assetID] = newTextureAsset;
		}
		else if (currentAssetPath.extension() == ".py") {
			Script* newScriptAsset = new Script(currentAssetPath.string());
			newScriptAsset->SetAssetType(AssetType::SCRIPT);
			newScriptAsset->SetID(Arcane::Core::UUID(assetID));
			m_Assets[assetID] = newScriptAsset;
		}
		return true;
	}

	bool AssetDatabase::CheckMetaInfo(std::filesystem::path currentMetaPath)
	{
		return std::filesystem::exists(currentMetaPath);
	}

	void AssetDatabase::GenerateMetaFile(std::filesystem::path metaPath, uint64_t newID)
	{
		// Generate ID 
		std::ofstream o(metaPath.string().c_str());

		// Create json layout 
		nlohmann::json meta_file;
		meta_file["id"] = newID;

		// save under .arcmeta file
		o << meta_file << std::endl;
	}

	uint64_t AssetDatabase::GetMetaID(std::filesystem::path currentMetaPath)
	{
		nlohmann::json metaJson;
		std::ifstream i(currentMetaPath.string().c_str());
		i >> metaJson;

		return metaJson["id"];
	}

	bool AssetDatabase::GenerateDefaultAssets()
	{
		// Create default texture
		Arcane::Core::UUID defaultTextureId = Arcane::Core::UUID();
		Texture* defaultTexture =  Texture::Create("./src/Assets/Textures/default.png");
		defaultTexture->SetAssetType(AssetType::TEXTURE);
		defaultTexture->SetID(defaultTextureId);
		m_Assets[defaultTextureId] = defaultTexture;
		m_DefaultAssets["DefaultTexture"] = (uint64_t)defaultTextureId;

		// Create default material
		uint64_t defaultMaterialId = Arcane::Core::UUID();
		Material* defaultMaterial = Material::Create(ShaderLibrary::GetShader("Mesh"));
		defaultTexture->SetAssetType(AssetType::MATERIAL);
		defaultTexture->SetID(defaultMaterialId);
		m_Assets[defaultMaterialId] = defaultMaterial;
		m_DefaultAssets["MeshMaterial"] = (uint64_t)defaultMaterialId;
		
		return true;
	}
}
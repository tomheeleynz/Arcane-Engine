#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <time.h>
#include <cstdlib>

#include "MeshAsset.h"
#include "AssetDatabase.h"


namespace Arcane
{
	AssetDatabase::AssetDatabase(std::filesystem::path assetDir)
	{
		m_AssetDirPath = assetDir;
		srand(time(0));
	}

	Asset* AssetDatabase::GetAsset(int id)
	{
		return m_Assets[id];
	}

	bool AssetDatabase::GenerateDatabase()
	{
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
		int assetID = 0;
		if (!CheckMetaInfo(metaPath)) {
			assetID = rand();
			GenerateMetaFile(metaPath, assetID);
		}
		else {
			assetID = GetMetaID(metaPath);
		}

		// Get File extension
		if (currentAssetPath.extension() == ".obj") {
			MeshAsset* meshAsset = new MeshAsset(currentAssetPath);
			meshAsset->SetID(assetID);
			m_Assets[assetID] = meshAsset;
		}
		else if (currentAssetPath.extension() == ".fbx") {
			// Generate Mesh Asset
		}
		return true;
	}

	bool AssetDatabase::CheckMetaInfo(std::filesystem::path currentMetaPath)
	{
		return std::filesystem::exists(currentMetaPath);
	}

	void AssetDatabase::GenerateMetaFile(std::filesystem::path metaPath, int newID)
	{
		// Generate ID 
		std::ofstream o(metaPath.string().c_str());

		// Create json layout 
		nlohmann::json meta_file;
		meta_file["id"] = newID;

		// save under .arcmeta file
		o << meta_file << std::endl;
	}

	int AssetDatabase::GetMetaID(std::filesystem::path currentMetaPath)
	{
		nlohmann::json metaJson;
		std::ifstream i(currentMetaPath.string().c_str());
		i >> metaJson;

		return metaJson["id"];
	}
}
#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>

#include "Asset.h"

namespace Arcane
{
	class AssetDatabase
	{
	public:
		AssetDatabase(std::filesystem::path assetDir);
		
		Asset* GetAsset(int id);
		Asset* GetDefaultAsset(std::string assetName);
		
		bool GenerateDatabase();

		bool GenerateAsset(std::filesystem::path currentAssetPath);
		
		bool CheckMetaInfo(std::filesystem::path currentMetaPath);
		void GenerateMetaFile(std::filesystem::path metaPath, int newID);

		int GetMetaID(std::filesystem::path metaPath);
	private:
		bool GenerateDefaultAssets();
	
	private:
		std::unordered_map<std::string, Asset*> m_DefaultAssets;
		std::unordered_map<uint64_t, Asset*> m_Assets;
		std::filesystem::path m_AssetDirPath;
	};
}
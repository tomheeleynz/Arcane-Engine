#pragma once

#include <unordered_map>
#include <filesystem>

#include "Asset.h"

namespace Arcane
{
	class AssetDatabase
	{
	public:
		AssetDatabase(std::filesystem::path assetDir);
		
		Asset* GetAsset(int id);
		
		bool GenerateDatabase();
		bool GenerateAsset(std::filesystem::path currentAssetPath);
		
		bool CheckMetaInfo(std::filesystem::path currentMetaPath);
		void GenerateMetaFile(std::filesystem::path metaPath, int newID);

		int GetMetaID(std::filesystem::path metaPath);
	private:
		std::unordered_map<int, Asset*> m_Assets;
		std::filesystem::path m_AssetDirPath;
	};
}
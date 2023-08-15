#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>

#include "Asset.h"
#include "Arcane/Renderer/Texture.h"


namespace Arcane
{
	class AssetDatabase
	{
	public:
		struct DependentAssetSpec
		{
			AssetType type;
			std::filesystem::path path;
		};

		AssetDatabase(std::filesystem::path assetDir);
		
		Asset* GetAsset(uint64_t id);
		Asset* GetDefaultAsset(std::string assetName);

		std::unordered_map<uint64_t, Asset*> GetAssetMap() { return m_Assets; };
		
		bool GenerateDatabase();
		bool GenerateAsset(std::filesystem::path currentAssetPath, bool dependent);
		
		bool CheckMetaInfo(std::filesystem::path currentMetaPath);
		void GenerateMetaFile(std::filesystem::path metaPath, uint64_t newID);

		uint64_t GetMetaID(std::filesystem::path metaPath);

		void AddAsset(uint64_t id, Asset* asset);
	private:
		bool GenerateDefaultAssets();
		void ParseTextureMetafile(Texture* texture, std::filesystem::path metaPath);
	
	private:
		std::unordered_map<std::string, uint64_t> m_DefaultAssets;
		std::unordered_map<uint64_t, Asset*> m_Assets;
		std::vector<DependentAssetSpec> m_DependentAssets;
		
		std::filesystem::path m_AssetDirPath;
	};
}
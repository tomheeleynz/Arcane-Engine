#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include <map>

#include "Arcane/Core/Application.h"

namespace Arcane
{
	struct AssetPackAsset
	{
		std::vector<char> binaryBlob;
		std::string json;
	};

	class AssetPack
	{
	public:
		AssetPack(AssetDatabase* database);

		void Serialize(std::filesystem::path path);
		void Deserialize(std::filesystem::path);

		std::pair<uint64_t, Asset*> UnpackAsset(AssetType type, std::ifstream& i);
	private:
		Asset* UnpackTexture(std::ifstream& i);
		Asset* UnpackScript(std::ifstream& i);
		Asset* UnpackShader(std::ifstream& i);

	private:
		std::filesystem::path m_Path;
		AssetDatabase* m_AssetDatabase;
	};
}
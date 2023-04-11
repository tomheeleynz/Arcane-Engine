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

		// Ofstream
		// std::ofstream pack("C:\\Projects\\BasicGame\\BasicGame.arcanepack", std::ios::out | std::ios::binary);


	}

	void AssetPack::Deserialize()
	{

	}
}
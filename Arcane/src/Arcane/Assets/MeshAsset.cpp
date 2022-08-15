#include "MeshAsset.h"

namespace Arcane
{ 
	MeshAsset::MeshAsset(std::filesystem::path meshPath)
	{
		m_Mesh = new Mesh(meshPath.string());
		SetAssetType(AssetType::MESH);
	}
}
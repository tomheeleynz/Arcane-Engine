#include "MeshAsset.h"

namespace Arcane
{ 
	MeshAsset::MeshAsset(std::filesystem::path meshPath)
	{	
		m_MeshPath = meshPath;
		SetAssetType(AssetType::MESH);
	}

	void MeshAsset::LoadAsset()
	{
		m_Mesh = new Mesh(m_MeshPath.string());
	}
}
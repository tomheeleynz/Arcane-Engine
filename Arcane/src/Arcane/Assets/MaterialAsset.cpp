#include "MaterialAsset.h"

namespace Arcane
{
	MaterialAsset::MaterialAsset(std::string filepath)
	{
		SetAssetType(AssetType::MATERIAL);
		m_Filepath = filepath;
	}

	MaterialAsset::MaterialAsset(Material* material)
	{
		SetAssetType(AssetType::MATERIAL);
		m_Material = material;
	}

	Material* MaterialAsset::GetMaterial()
	{
		return m_Material;
	}

	void MaterialAsset::LoadAsset()
	{
		if (m_Material == nullptr)
		{
		}
	}
}
#pragma once
#include "Asset.h"
#include "Arcane/Renderer/Material.h"

namespace Arcane
{
	class MaterialAsset : public Asset
	{
	public:
		MaterialAsset(std::string filepath);
		MaterialAsset(Material* material);

		Material* GetMaterial();
		void LoadAsset() override;
	private:
		Material* m_Material = nullptr;
		std::string m_Filepath;
	};
}
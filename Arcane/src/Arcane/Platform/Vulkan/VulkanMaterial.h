#pragma once

#include "Arcane/Renderer/Material.h"

namespace Arcane
{
	class VulkanMaterial: public Material
	{
	public:
		VulkanMaterial(Shader* shader);

		Shader* GetShader() override;
		void SetShader(Shader* shader) override;
	private:
		Shader* m_Shader;
	};
}
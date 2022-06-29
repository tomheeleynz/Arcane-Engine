#pragma once

#include "Arcane/Renderer/Material.h"
#include "VulkanShader.h"

namespace Arcane
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(Shader* shader);

		std::vector<ShaderVariable> GetShaderVariables();
	private:
		Shader* m_Shader;
	};
}
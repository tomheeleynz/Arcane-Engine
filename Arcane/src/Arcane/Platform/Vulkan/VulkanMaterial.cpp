#include "VulkanMaterial.h"

namespace Arcane
{
	VulkanMaterial::VulkanMaterial(Shader* shader)
	{
		m_Shader = shader;
		
		// Reflect Shader to get descriptor info
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(shader);
		vulkanShader->Reflect();
	}

	std::vector<ShaderVariable> VulkanMaterial::GetShaderVariables()
	{
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader);
		return m_Shader->GetShaderVariables();
	}
}
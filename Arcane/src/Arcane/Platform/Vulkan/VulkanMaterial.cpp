#include "VulkanMaterial.h"
#include "VulkanShader.h"

namespace Arcane
{
	VulkanMaterial::VulkanMaterial(Shader* shader)
	{
		m_Shader = shader;
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader);
		vulkanShader->Reflect();
	}

	Shader* VulkanMaterial::GetShader()
	{
		return m_Shader;
	}

	void VulkanMaterial::SetShader(Shader* shader)
	{
		m_Shader = shader;
	}

}
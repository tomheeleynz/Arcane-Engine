#include "VulkanMaterial.h"

namespace Arcane
{
	VulkanMaterial::VulkanMaterial(Shader* shader)
	{
		m_Shader = shader;
		
		// Reflect Shader to get descriptor info
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(shader);

		// Get Material descriptor, then add uniform buffer to it
		m_UniformBuffer = UniformBuffer::Create(vulkanShader->GetMaterialSize());

		// Create internal structure
		m_UniformBufferMemory = new float[vulkanShader->GetMaterialSize()];
	}

	void VulkanMaterial::WriteVec3(uint32_t offset, glm::vec3 value)
	{
		m_UniformBufferMemory[offset] = value.x;
		m_UniformBufferMemory[offset + 1] = value.y;
		m_UniformBufferMemory[offset + 2] = value.z;
	}

	glm::vec3 VulkanMaterial::GetVec3(uint32_t offset)
	{
		return glm::vec3(
			m_UniformBufferMemory[offset], 
			m_UniformBufferMemory[offset + 1], 
			m_UniformBufferMemory[offset + 2]
		);
	}

	std::vector<ShaderVariable> VulkanMaterial::GetMaterialVariables()
	{
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader);
		return m_Shader->GetMaterialVariables();
	}

	DescriptorSet* VulkanMaterial::GetDescriptorSet()
	{
		return m_DescriptorSet;
	}

	UniformBuffer* VulkanMaterial::GetUniformBuffer()
	{
		return m_UniformBuffer;
	}

}
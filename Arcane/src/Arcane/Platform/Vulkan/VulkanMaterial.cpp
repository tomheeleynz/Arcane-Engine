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

		m_DescriptorSet = vulkanShader->GetMaterialDescriptor();
		m_DescriptorSet->AddUniformBuffer(m_UniformBuffer, 2, 0);

		// Create internal structure
		m_UniformBufferMemory = new float[3];
	}

	void VulkanMaterial::UpdateMaterialData()
	{
		m_UniformBuffer->WriteData((void*)m_UniformBufferMemory, m_Shader->GetMaterialSize());
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

	//////////////////////////////////////////////////////////////////
	////// Getters and Setters
	//////////////////////////////////////////////////////////////////

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

}
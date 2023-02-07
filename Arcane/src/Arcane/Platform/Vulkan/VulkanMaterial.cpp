#include "VulkanMaterial.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	VulkanMaterial::VulkanMaterial(Shader* shader)
	{
		if (shader == nullptr) return;

		m_Shader = shader;

		// Reflect Shader to get descriptor info
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(shader);

		// Get Descriptor Sets From Shader

		// 
	}

	void VulkanMaterial::UpdateMaterialData()
	{
	}

	Shader* VulkanMaterial::GetShader()
	{
		return m_Shader;
	}

	void VulkanMaterial::SetShader(Shader* shader)
	{
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
	void VulkanMaterial::WriteVec3(int binding, uint32_t offset, glm::vec3 value)
	{
		m_UniformBuffersData[binding].second[offset] = value.x;
		m_UniformBuffersData[binding].second[offset + 1] = value.y;
		m_UniformBuffersData[binding].second[offset + 2] = value.z;
	}

	void VulkanMaterial::WriteTexture(int binding, Texture* texture)
	{
		m_MaterialTextures[binding] = texture;
		m_DescriptorSet->AddImageSampler(m_MaterialTextures[binding], 2, binding);
	}

	void VulkanMaterial::WriteTexture(int binding, glm::vec4 color)
	{
		// m_MaterialTextures[binding]->UpdateTexture(color.x, color.y, color.z, color.w);
	}

	glm::vec3 VulkanMaterial::GetVec3(int binding, uint32_t offset)
	{
		return glm::vec3(
			m_UniformBuffersData[binding].second[offset],
			m_UniformBuffersData[binding].second[offset + 1],
			m_UniformBuffersData[binding].second[offset + 2]
		);
	}

	Texture* VulkanMaterial::GetTexture(int binding)
	{
		return m_MaterialTextures[binding];
	}

}
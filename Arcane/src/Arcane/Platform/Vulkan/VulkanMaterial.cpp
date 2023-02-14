#include "VulkanMaterial.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	VulkanMaterial::VulkanMaterial(Shader* shader)
	{
		if (shader == nullptr) return;
		
		m_Shader = shader;
		SetShader(m_Shader);
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
		m_Shader = shader;

		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader);
		std::vector<ShaderSet> shaderSets = m_Shader->GetShaderSets();

		for (int i = 0; i < shaderSets.size(); i++) {
			ShaderSet& set = shaderSets[i];

			// We only care about the material set in this instance
			if (set.SetNumber == 2) {
				m_Set = set;
			}
		}
	}

	ShaderSet& VulkanMaterial::GetMaterialSet()
	{
		return m_Set;
	}

	DescriptorSet* VulkanMaterial::GetDescriptorSet()
	{
		return m_Shader->GetMaterialDescriptor();
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
		m_Shader->GetMaterialDescriptor()->AddImageSampler(m_MaterialTextures[binding], 2, binding);
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

	Pipeline* VulkanMaterial::GetPipeline()
	{
		return nullptr;
	}

}
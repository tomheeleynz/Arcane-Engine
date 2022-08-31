#include "VulkanMaterial.h"
#include "Arcane/Core/Application.h"
#include "Arcane/Assets/TextureAsset.h"

namespace Arcane
{
	VulkanMaterial::VulkanMaterial(Shader* shader)
	{
		m_Shader = shader;

		// Reflect Shader to get descriptor info
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(shader);
		m_DescriptorSet = vulkanShader->GetMaterialDescriptor();

		std::map<int, int> m_UniformBufferSizeMap;

		// Get Default Texture
		Texture* defaultTexture = static_cast<TextureAsset*>(Application::Get().GetAssetDatabase().GetDefaultAsset("DefaultTexture"))->GetTexture();

		// Actually parse these variables
		for (ShaderVariable variable : GetMaterialVariables())
		{
			if (variable.Type == ShaderVariableType::Vec3) {
				m_UniformBuffersData[variable.binding].first += 3;
			}

			if (variable.Type == ShaderVariableType::Sampler) {
				m_MaterialTextures[variable.binding] = defaultTexture;
				m_DescriptorSet->AddImageSampler(m_MaterialTextures[variable.binding], 2, variable.binding);
			}
		}

		for (auto& uniformBufferData : m_UniformBuffersData) {
			// Create the memory
			uniformBufferData.second.second = new float[uniformBufferData.second.first];
			m_UniformBuffers[uniformBufferData.first] = UniformBuffer::Create(sizeof(float) * uniformBufferData.second.first);
			m_DescriptorSet->AddUniformBuffer(m_UniformBuffers[uniformBufferData.first], 2, uniformBufferData.first);
		}
	}

	void VulkanMaterial::UpdateMaterialData()
	{
		for (auto& uniformBuffer : m_UniformBuffers) {
			uniformBuffer.second->WriteData((void*)m_UniformBuffersData[uniformBuffer.first].second, m_UniformBuffersData[uniformBuffer.first].first * sizeof(float));
		}
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
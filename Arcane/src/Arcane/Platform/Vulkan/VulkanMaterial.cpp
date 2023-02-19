#include "VulkanMaterial.h"
#include "VulkanSet.h"
#include "VulkanSet.h"
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
		std::vector<DescriptorSet*> descriptorSets = vulkanShader->GetDescriptorSets();
		
		DescriptorSet* materialSet = nullptr;
		{
			for (int i = 0; i < descriptorSets.size(); i++) {
				VulkanSet* vulkanSet = static_cast<VulkanSet*>(descriptorSets[i]);
				
				if (vulkanSet->GetSetNumber() == 2) {
					materialSet = descriptorSets[i];
					m_MaterialDescriptorSet = descriptorSets[i];
				}
			}
		}

		for (int i = 0; i < shaderSets.size(); i++) {
			ShaderSet& set = shaderSets[i];

			// We only care about the material set in this instance
			if (set.SetNumber == 2) {
				m_Set = set;
			
				for (int j = 0; j < shaderSets[i].Bindings.size(); j++) {
					ShaderBinding& binding = shaderSets[i].Bindings[j];
					
					if (binding.Type == ShaderBindingType::UNIFORM) {
						m_UniformBuffers[binding.Binding] = UniformBuffer::Create(binding.Size * sizeof(float));
						m_UniformBuffersData[binding.Binding] = new float[binding.Size / 4];

						for (int k = 0; k < binding.Size / 4; k++) {
							m_UniformBuffersData[binding.Binding][k] = 0.0f;
						}

						m_MaterialDescriptorSet->AddUniformBuffer(m_UniformBuffers[binding.Binding], set.SetNumber, binding.Binding);
					}
				}
			}
		}

		m_VertexDescriptor = VertexDescriptor::Create({
			VertexType::float3,
			VertexType::float3,
			VertexType::float2
		});
	}

	ShaderSet& VulkanMaterial::GetMaterialSet()
	{
		return m_Set;
	}

	void VulkanMaterial::SetGlobalData(DescriptorSet* set)
	{
		VulkanShader* shader = static_cast<VulkanShader*>(m_Shader);
		std::vector<DescriptorSet*> descriptorSets = shader->GetDescriptorSets();

		for (int i = 0; i < descriptorSets.size(); i++) {
			VulkanSet* set = static_cast<VulkanSet*>(descriptorSets[i]);
			if (set->GetSetNumber() == 0) {
				descriptorSets[i] = set;
				break;
			}
		}
	}

	void VulkanMaterial::SetFrameData(DescriptorSet* set)
	{
		VulkanShader* shader = static_cast<VulkanShader*>(m_Shader);
		std::vector<DescriptorSet*> descriptorSets = shader->GetDescriptorSets();

		for (int i = 0; i < descriptorSets.size(); i++) {
			VulkanSet* set = static_cast<VulkanSet*>(descriptorSets[i]);
			if (set->GetSetNumber() == 1) {
				descriptorSets[i] = set;
				break;
			}
		}
	}

	void VulkanMaterial::SetDrawData(DescriptorSet* set)
	{
		VulkanShader* shader = static_cast<VulkanShader*>(m_Shader);
		std::vector<DescriptorSet*> descriptorSets = shader->GetDescriptorSets();

		for (int i = 0; i < descriptorSets.size(); i++) {
			VulkanSet* set = static_cast<VulkanSet*>(descriptorSets[i]);
			if (set->GetSetNumber() == 3) {
				descriptorSets[i] = set;
				break;
			}
		}
	}

	DescriptorSet* VulkanMaterial::GetDescriptorSet()
	{
		return m_Shader->GetMaterialDescriptor();
	}

	std::vector<DescriptorSet*> VulkanMaterial::GetDescriptorSets()
	{
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader);
		return vulkanShader->GetDescriptorSets();
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
		offset = offset / 4;
		glm::vec4 fullValue = glm::vec4(value, 0.0f);
		m_UniformBuffersData[binding][offset] = fullValue.x;
		m_UniformBuffersData[binding][offset + 1] = fullValue.y;
		m_UniformBuffersData[binding][offset + 2] = fullValue.z;
		m_UniformBuffersData[binding][offset + 3] = fullValue.w;
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
		offset = offset / 4;
		return glm::vec3(
			m_UniformBuffersData[binding][offset],
			m_UniformBuffersData[binding][offset + 1],
			m_UniformBuffersData[binding][offset + 2]
		);
	}

	Texture* VulkanMaterial::GetTexture(int binding)
	{
		return m_MaterialTextures[binding];
	}

	void VulkanMaterial::SetRenderPass(RenderPass* renderPass)
	{
		if (m_RenderPass != nullptr)
			return;

		m_RenderPass = renderPass;
		RecreatePipeline();
	}

	Pipeline* VulkanMaterial::GetPipeline()
	{
		return m_Pipeline;
	}

	void VulkanMaterial::RecreatePipeline()
	{
		VulkanShader* vulkanShader = static_cast<VulkanShader*>(m_Shader);
		std::vector<DescriptorSet*> descriptorSets = vulkanShader->GetDescriptorSets();

		PipelineSpecification specs;
		specs.DescriptorSets = descriptorSets;
		specs.shader = m_Shader;
		specs.renderPass = m_RenderPass;
		specs.descriptor = m_VertexDescriptor;

		m_Pipeline = Pipeline::Create(specs);
	}
}
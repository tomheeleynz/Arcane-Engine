#pragma once

#include "Arcane/Renderer/Material.h"
#include "VulkanShader.h"
#include "VulkanTexture.h"

namespace Arcane
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(Shader* shader);

		DescriptorSet* GetDescriptorSet() override;
		UniformBuffer* GetUniformBuffer() override;

		// Get Material Variables from material buffer
		virtual glm::vec3 GetVec3(uint32_t offset) override;

		// Write Material Variables to material buffer
		virtual void WriteVec3(uint32_t offset, glm::vec3 value) override;

		void UpdateMaterialData() override;

		std::vector<ShaderVariable> GetMaterialVariables();
	private:
		// Things to render material
		Shader* m_Shader;
		DescriptorSet* m_DescriptorSet;
		UniformBuffer* m_UniformBuffer;

		// Memory to hold material variables
		float* m_UniformBufferMemory;
		std::vector<VulkanTexture> m_MaterialTextures;
	};
}
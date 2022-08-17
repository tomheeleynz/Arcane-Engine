#pragma once

#include "Arcane/Renderer/Material.h"
#include "Arcane/Renderer/UniformBuffer.h"
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
		virtual glm::vec3 GetVec3(int binding, uint32_t offset) override;

		// Write Material Variables to material buffer
		virtual void WriteVec3(int binding,uint32_t offset, glm::vec3 value) override;

		void UpdateMaterialData() override;

		std::vector<ShaderVariable> GetMaterialVariables();
	private:
		// Things to render material
		Shader* m_Shader;
		DescriptorSet* m_DescriptorSet;
		UniformBuffer* m_UniformBuffer;

		// Memory to hold material variables
		float* m_UniformBufferMemory;

		// Able to keep track of things
		std::map<int, std::pair<int, float*>> m_UniformBuffersData;
		std::map<int, UniformBuffer*> m_UniformBuffers;
		
		// For textures
		std::map<int, Texture*> m_MaterialTextures;
	};
}
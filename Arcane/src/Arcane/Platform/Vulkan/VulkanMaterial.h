#pragma once

#include "Arcane/Renderer/Material.h"
#include "Arcane/Renderer/UniformBuffer.h"

#include "VulkanPipeline.h"
#include "VulkanShader.h"
#include "VulkanTexture.h"

namespace Arcane
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(Shader* shader);

		DescriptorSet* GetDescriptorSet() override;
		std::vector<DescriptorSet*> GetDescriptorSets() override;
		UniformBuffer* GetUniformBuffer() override;
		Pipeline* GetPipeline() override;
		void SetRenderPass(RenderPass* renderPass) override;

		// Get Material Variables from material buffer
		virtual glm::vec3 GetVec3(int binding, uint32_t offset) override;
		virtual Texture* GetTexture(int binding) override;

		// Write Material Variables to material buffer
		virtual void WriteVec3(int binding,uint32_t offset, glm::vec3 value) override;
		virtual void WriteTexture(int binding, Texture* texture) override;
		virtual void WriteTexture(int binding, glm::vec4 color) override;

		void UpdateMaterialData() override;

		virtual Shader* GetShader() override;
		virtual void SetShader(Shader* shader) override;

		ShaderSet& GetMaterialSet() override;

		void SetGlobalData(DescriptorSet* set) override;
		void SetFrameData(DescriptorSet* set) override;
		void SetDrawData(DescriptorSet* set) override;

	private:
		void RecreatePipeline();
	private:
		// Material Set from shader, if present
		ShaderSet m_Set;

		// Things to render material
		Shader* m_Shader = nullptr;
		DescriptorSet* m_DescriptorSet;
		UniformBuffer* m_UniformBuffer;

		// Memory to hold material variables
		float* m_UniformBufferMemory;

		// Able to keep track of things
		std::map<int, float*> m_UniformBuffersData;
		std::map<int, UniformBuffer*> m_UniformBuffers;
		
		// For textures
		std::map<int, Texture*> m_MaterialTextures;

		// Pipeline
		Pipeline* m_Pipeline = nullptr;
		RenderPass* m_RenderPass = nullptr;
		VertexDescriptor* m_VertexDescriptor;
	};
}
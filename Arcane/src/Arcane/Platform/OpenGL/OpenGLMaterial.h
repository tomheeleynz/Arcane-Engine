#pragma once

#include "Arcane/Renderer/Material.h"

namespace Arcane
{
	class OpenGLMaterial : public Material 
	{
	public:
		OpenGLMaterial(Shader* shader);

		virtual ShaderSet& GetMaterialSet() override;
		virtual DescriptorSet* GetDescriptorSet() override;
		virtual std::vector<DescriptorSet*> GetDescriptorSets() override;

		virtual UniformBuffer* GetUniformBuffer() override;

		virtual Pipeline* GetPipeline() override;
		virtual void SetRenderPass(RenderPass* renderPass) override;

		// Get Material Variables from material buffer
		virtual glm::vec3 GetVec3(int binding, uint32_t offset) override;
		virtual Texture* GetTexture(int binding) override;

		// Write Material Variables to material buffer
		virtual void WriteVec3(int binding, uint32_t offset, glm::vec3 value) override;

		// Write Texture (both image and color)
		virtual void WriteTexture(int binding, Texture* texture) override;
		virtual void WriteTexture(int binding, glm::vec4 color) override;

		// Update Material data from internal structures
		virtual void UpdateMaterialData() override;

		// Get the shader from the material
		virtual Shader* GetShader() override;
		virtual void SetShader(Shader* shader) override;
		
		virtual void SetGlobalData(DescriptorSet* set) override;
		virtual void SetFrameData(DescriptorSet* set) override;
		virtual void SetDrawData(DescriptorSet* set) override;
	private:
		Shader* m_Shader;
	
	};
}
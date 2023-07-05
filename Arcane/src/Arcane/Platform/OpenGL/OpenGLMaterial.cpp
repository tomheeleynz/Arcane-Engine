#include "OpenGLMaterial.h"


namespace Arcane
{
	OpenGLMaterial::OpenGLMaterial(Shader* shader)
	{
		m_Shader = shader;
	}

	ShaderSet& OpenGLMaterial::GetMaterialSet()
	{
		ShaderSet newSet;
		return newSet;
	}

	DescriptorSet* OpenGLMaterial::GetDescriptorSet()
	{
		return nullptr;
	}

	std::vector<DescriptorSet*> OpenGLMaterial::GetDescriptorSets()
	{
		return std::vector<DescriptorSet*>();
	}

	UniformBuffer* OpenGLMaterial::GetUniformBuffer()
	{
		return nullptr;
	}

	Pipeline* OpenGLMaterial::GetPipeline()
	{
		return nullptr;
	}

	void OpenGLMaterial::SetRenderPass(RenderPass* renderPass)
	{


	}

	glm::vec3 OpenGLMaterial::GetVec3(int binding, uint32_t offset)
	{
		return glm::vec3();
	}
	Texture* OpenGLMaterial::GetTexture(int binding)
	{
		return nullptr;
	}

	void OpenGLMaterial::WriteVec3(int binding, uint32_t offset, glm::vec3 value)
	{

	}

	void OpenGLMaterial::WriteTexture(int binding, Texture* texture)
	{

	}

	void OpenGLMaterial::WriteTexture(int binding, glm::vec4 color)
	{

	}

	void OpenGLMaterial::UpdateMaterialData()
	{

	}

	Shader* OpenGLMaterial::GetShader()
	{
		return m_Shader;
	}

	void OpenGLMaterial::SetShader(Shader* shader)
	{
		m_Shader = shader;
	}

	void OpenGLMaterial::SetGlobalData(DescriptorSet* set)
	{

	}

	void OpenGLMaterial::SetFrameData(DescriptorSet* set)
	{ 

	}

	void OpenGLMaterial::SetDrawData(DescriptorSet* set)
	{

	}
}
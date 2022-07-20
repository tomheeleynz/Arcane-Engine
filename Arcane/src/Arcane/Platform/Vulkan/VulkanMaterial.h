#pragma once

#include "Arcane/Renderer/Material.h"
#include "VulkanShader.h"

namespace Arcane
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(Shader* shader);

		DescriptorSet* GetDescriptorSet() override;
		UniformBuffer* GetUniformBuffer() override;

		std::vector<ShaderVariable> GetShaderVariables();
	private:
		Shader* m_Shader;
		DescriptorSet* m_DescriptorSet;
		UniformBuffer* m_UniformBuffer;
	};
}
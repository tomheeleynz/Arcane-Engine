#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>
#include <spirv_reflect.h>

#include "Arcane/Renderer/Shader.h"

namespace Arcane 
{

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(std::string vertexShader, std::string fragmentShader);
		virtual std::vector<ShaderVariable> GetShaderVariables() { return m_ShaderVariables; };

		VkShaderModule GetVertexShaderModule() { return m_VertexShaderModule; }
		VkShaderModule GetFragmentShaderModule() { return m_FragShaderModule; }
		DescriptorSet* GetMaterialDescriptor() override;
		
		void Reflect();
	private:
		void ReflectModule(std::vector<char> byteCode, VkShaderModule module);
	private:
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragShaderModule;

		DescriptorSet* m_MaterialSet = nullptr;

		std::vector<char> m_VertexByteCode;
		std::vector<char> m_FragmentByteCode;

		std::vector<ShaderVariable> m_ShaderVariables;
	};
}
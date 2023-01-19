#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>
#include <spirv_reflect.h>
#include <shaderc/shaderc.h>

#include "Arcane/Renderer/Shader.h"

namespace Arcane 
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(std::string vertexShader, std::string fragmentShader);
		VulkanShader(std::string shaderFile);

		virtual std::vector<ShaderVariable> GetMaterialVariables() { return m_MaterialVariables; };
		virtual uint32_t GetMaterialSize() { return m_MaterialSize; }

		VkShaderModule GetVertexShaderModule() { return m_VertexShaderModule; }
		VkShaderModule GetFragmentShaderModule() { return m_FragShaderModule; }
		DescriptorSet* GetMaterialDescriptor() override;
		
		void Reflect();
	private:
		void ReflectModule(std::vector<uint32_t> byteCode, VkShaderModule module);
		std::vector<uint32_t> CompileShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false);
	private:
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragShaderModule;

		DescriptorSet* m_MaterialSet = nullptr;
		uint32_t m_MaterialSize = 0;

		std::vector<char> m_VertexByteCode;
		std::vector<char> m_FragmentByteCode;

		std::vector<ShaderVariable> m_MaterialVariables;
	};
}
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

		virtual std::vector<ShaderSet> GetShaderSets() { return m_ShaderSets; };
		virtual uint32_t GetMaterialSize() { return m_MaterialSize; }

		VkShaderModule GetVertexShaderModule() { return m_VertexShaderModule; }
		VkShaderModule GetFragmentShaderModule() { return m_FragShaderModule; }
		DescriptorSet* GetMaterialDescriptor() override;
		std::vector<DescriptorSet*> GetDescriptorSets() { return m_DescriptorSets; }

		void FindShaderMembers(SpvReflectBlockVariable& block, ShaderMember& member);

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

		std::vector<ShaderSet> m_ShaderSets;

		// Descriptor Sets of Shader
		std::vector<DescriptorSet*> m_DescriptorSets;
		DescriptorSet* m_MaterialDescriptorSet;
	};
}
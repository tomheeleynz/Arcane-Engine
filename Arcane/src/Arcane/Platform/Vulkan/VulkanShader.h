#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>
#include <spirv_reflect.h>

#include "Arcane/Renderer/Shader.h"

namespace Arcane {
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(std::string vertexShader, std::string fragmentShader);

		VkShaderModule GetVertexShaderModule() { return m_VertexShaderModule; }
		VkShaderModule GetFragmentShaderModule() { return m_FragShaderModule; }
		void Reflect();
	private:
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragShaderModule;

		std::vector<char> m_VertexByteCode;
		std::vector<char> m_FragmentByteCode;
	};
}
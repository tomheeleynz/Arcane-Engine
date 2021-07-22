#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>
#include "Renderer/Shader.h"

namespace Arcane {
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(std::string vertexShader, std::string fragmentShader);

		VkShaderModule GetVertexShaderModule() { return m_VertexShaderModule; }
		VkShaderModule GetFragmentShaderModule() { return m_FragShaderModule; }
	private:
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragShaderModule;
	};
}
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanShader.h"

Shader* Shader::Create(std::string vertexShader, std::string fragmentShader)
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::Vulkan: return new VulkanShader(vertexShader, fragmentShader);
	default: return nullptr;
	}
}

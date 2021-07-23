#include "Shader.h"
#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Platform/Vulkan/VulkanShader.h"

namespace Arcane {
	Shader* Shader::Create(std::string vertexShader, std::string fragmentShader)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanShader(vertexShader, fragmentShader);
		default: return nullptr;
		}
	}
}

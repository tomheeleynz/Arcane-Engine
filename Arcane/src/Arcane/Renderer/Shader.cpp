#include "Shader.h"
#include "Arcane/Renderer/Renderer.h"

#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanShader.h"
#endif

namespace Arcane {
	Shader* Shader::Create(std::string vertexShader, std::string fragmentShader)
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanShader(vertexShader, fragmentShader);
#endif
		default: return nullptr;
		}
	}
}

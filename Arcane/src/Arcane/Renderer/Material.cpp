#include "Material.h"
#include "Renderer.h"

#include "Arcane/Platform/Vulkan/VulkanMaterial.h"
#include "Arcane/Platform/OpenGL/OpenGLMaterial.h"

namespace Arcane
{
	Material* Material::Create(Shader* shader)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanMaterial(shader);
		case RendererAPIType::OpenGL: return new OpenGLMaterial(shader);
		default: return nullptr;
		}
	}
}
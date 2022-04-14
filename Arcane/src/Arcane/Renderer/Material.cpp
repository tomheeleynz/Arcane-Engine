#include "Material.h"
#include "Renderer.h"

#include "Arcane/Platform/Vulkan/VulkanMaterial.h"

namespace Arcane
{
	Material* Material::Create(Shader* shader)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanMaterial(shader);
		default: return nullptr;
		}
	}
}
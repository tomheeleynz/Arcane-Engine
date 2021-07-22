#include "GraphicsContext.h"
#include "Renderer/RendererAPI.h"

// -- Vulkan Context
#include "Platform/Vulkan/VulkanContext.h"

namespace Arcane {

	GraphicsContext* GraphicsContext::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanContext();
		default: return nullptr;
		}
	}
}
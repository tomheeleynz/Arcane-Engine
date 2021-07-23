#include "GraphicsContext.h"
#include "Arcane/Renderer/RendererAPI.h"

// -- Vulkan Context
#include "Arcane/Platform/Vulkan/VulkanContext.h"

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
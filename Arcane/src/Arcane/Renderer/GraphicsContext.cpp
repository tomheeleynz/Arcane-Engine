#include "GraphicsContext.h"
#include "Arcane/Renderer/RendererAPI.h"

// -- Vulkan Context
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanContext.h"
#endif

#ifdef ARCANE_APPLE
#include "Arcane/Platform/Metal/MetalContext.h"
#endif

namespace Arcane {

	GraphicsContext* GraphicsContext::Create()
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanContext();
#endif
#ifdef ARCANE_APPLE
        case RendererAPIType::Metal: return new MetalContext();
#endif
		default: return nullptr;
		}
	}
}

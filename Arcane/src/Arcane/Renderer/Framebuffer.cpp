#include "Framebuffer.h"
#include "Renderer.h"

// Platform
// -- Vulkan
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanFramebuffer.h"
#endif

namespace Arcane {

	Framebuffer* Framebuffer::Create(FramebufferSpecifications& specs)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None: return nullptr;
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanFramebuffer(specs);
#endif
		default: return nullptr;
		}
	}
}

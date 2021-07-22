#include "Framebuffer.h"
#include "Renderer.h"

// Platform
// -- Vulkan
#include "Platform/Vulkan/VulkanFramebuffer.h"

namespace Arcane {

	Framebuffer* Framebuffer::Create(FramebufferSpecifications& specs)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::Vulkan: return new VulkanFramebuffer(specs);
		default: return nullptr;
		}
	}
}
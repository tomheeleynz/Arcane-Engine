#include "Framebuffer.h"
#include "Renderer.h"

// Platform
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanFramebuffer.h"

// -- OpenGL
#include "Arcane/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Arcane {

	Framebuffer* Framebuffer::Create(FramebufferSpecifications& specs)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::Vulkan: return new VulkanFramebuffer(specs);
		case RendererAPIType::OpenGL: return new OpenGLFramebuffer(specs);
		default: return nullptr;
		}
	}
}
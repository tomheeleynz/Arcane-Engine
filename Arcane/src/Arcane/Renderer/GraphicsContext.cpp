#include "GraphicsContext.h"
#include "Arcane/Renderer/RendererAPI.h"

// -- Vulkan Context
#include "Arcane/Platform/Vulkan/VulkanContext.h"


// -- OpenGL Context
#include "Arcane/Platform/OpenGL/OpenGLContext.h"

namespace Arcane {

	GraphicsContext* GraphicsContext::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanContext();
		case RendererAPIType::OpenGL: return new OpenGLContext();
		default: return nullptr;
		}
	}
}
#include "Arcane/Renderer/RenderPass.h"
#include "Arcane/Renderer/Renderer.h"

// -- Vulkam
#include "Arcane/Platform/Vulkan/VulkanRenderPass.h"

// -- OpenGL
#include "Arcane/Platform/OpenGL/OpenGLRenderPass.h"


namespace Arcane {

	RenderPass* RenderPass::Create(RenderPassSpecs specs)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanRenderPass(specs);
		case RendererAPIType::OpenGL: return new OpenGLRenderPass(specs);
		default:
			break;
		}
	}
}
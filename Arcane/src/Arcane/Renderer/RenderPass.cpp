#include "Arcane/Renderer/RenderPass.h"
#include "Arcane/Renderer/Renderer.h"

#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanRenderPass.h"
#endif

namespace Arcane {

	RenderPass* RenderPass::Create(RenderPassSpecs specs)
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanRenderPass(specs);
#endif
		default:
			break;
		}
	}
}

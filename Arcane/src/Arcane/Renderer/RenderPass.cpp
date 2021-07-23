#include "Arcane/Renderer/RenderPass.h"
#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Platform/Vulkan/VulkanRenderPass.h"

namespace Arcane {

	RenderPass* RenderPass::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanRenderPass();
		default:
			break;
		}
	}
}
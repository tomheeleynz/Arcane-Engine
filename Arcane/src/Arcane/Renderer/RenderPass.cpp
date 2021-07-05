#include "Renderer/RenderPass.h"
#include "Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanRenderPass.h"

RenderPass* RenderPass::Create()
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::Vulkan: return new VulkanRenderPass();
	default:
		break;
	}
}
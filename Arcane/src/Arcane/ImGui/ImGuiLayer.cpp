#include "ImGuiLayer.h"
#include "Renderer/Renderer.h"

// Platform
// -- Vulkan
#include "Platform/Vulkan/VulkanImGuiLayer.h"

namespace Arcane {
	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanImGuiLayer();
		case RendererAPIType::None: return nullptr;
		default: return nullptr;
		}
	}
}
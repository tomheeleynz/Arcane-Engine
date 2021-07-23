#include "ImGuiLayer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanImGuiLayer.h"

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
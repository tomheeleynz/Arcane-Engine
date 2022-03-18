#include "ImGuiLayer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform
// -- Vulkan
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanImGuiLayer.h"
#endif

namespace Arcane {
	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanImGuiLayer();
#endif
		case RendererAPIType::None: return nullptr;
		default: return nullptr;
		}
	}
}

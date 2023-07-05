#include "ImGuiLayer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanImGuiLayer.h"

// -- OpenGL
#include "Arcane/Platform/OpenGL/OpenGLImguiLayer.h"

namespace Arcane {
	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanImGuiLayer();
		case RendererAPIType::OpenGL: return new OpenGLImguiLayer();
		case RendererAPIType::None: return nullptr;
		default: return nullptr;
		}
	}
}
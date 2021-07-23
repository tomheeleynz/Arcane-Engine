#include "UniformBuffer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanUniformBuffer.h"

namespace Arcane {
	UniformBuffer* UniformBuffer::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer();
		case RendererAPIType::None: return nullptr;
		default: return nullptr;
		}
	}
}
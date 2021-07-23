#include "UniformBuffer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanUniformBuffer.h"

namespace Arcane {
	UniformBuffer* UniformBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer(size);
		case RendererAPIType::None: return nullptr;
		default: return nullptr;
		}
	}
}
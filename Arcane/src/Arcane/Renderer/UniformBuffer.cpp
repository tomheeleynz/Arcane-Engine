#include "RendererAPI.h"
#include "UniformBuffer.h"

// Platform
// -- Vulkam
#include "Arcane/Platform/Vulkan/VulkanUniformBuffer.h"

namespace Arcane
{
	UniformBuffer* UniformBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer(size);
		default:
			break;
		}
	}
}
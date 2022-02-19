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

	UniformBuffer* UniformBuffer::Create(Texture* texture, uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer(texture, size);
		default:
			break;
		}
	}

	UniformBuffer* UniformBuffer::Create(std::initializer_list<UniformDescriptor*> descriptors)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer(descriptors);
		default:
			break;
		}
	}
}
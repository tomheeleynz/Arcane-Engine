#include "RendererAPI.h"
#include "UniformBuffer.h"

// Platform
// -- Vulkam
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanUniformBuffer.h"
#endif

namespace Arcane
{
	UniformBuffer* UniformBuffer::Create(std::initializer_list<UniformDescriptor*> descriptors)
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer(descriptors);
#endif
		default:
			break;
		}
	}
}

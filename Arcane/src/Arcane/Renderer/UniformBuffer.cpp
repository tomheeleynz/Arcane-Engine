#include "RendererAPI.h"
#include "UniformBuffer.h"

// Platform
// -- Vulkam
#include "Arcane/Platform/Vulkan/VulkanUniformBuffer.h"

// -- OpenGl
#include "Arcane/Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Arcane
{
	UniformBuffer* UniformBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanUniformBuffer(size);
		case RendererAPIType::OpenGL: return new OpenGLUniformBuffer(size);
		default:
			break;
		}
	}
}
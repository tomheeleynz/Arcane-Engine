#include "Buffer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform Specific
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanBuffer.h"

// -- OpenGL
#include "Arcane/Platform/OpenGL/OpenGLBuffer.h"

namespace Arcane {

	////////////////////////////////////////////////////////
	//// Vertex Buffer
	////////////////////////////////////////////////////////
	VertexBuffer* VertexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanVertexBuffer(data, size);
		case RendererAPIType::OpenGL: return new OpenGLVertexBuffer(data, size);
		default: return nullptr;
		}
	}

	////////////////////////////////////////////////////////
	//// Index Buffer
	////////////////////////////////////////////////////////
	IndexBuffer* IndexBuffer::Create(void* data, uint32_t count)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanIndexBuffer(data, count);
		case RendererAPIType::OpenGL: return new OpenGLIndexBuffer(data, count);
		default: return nullptr;
		}
	}
}
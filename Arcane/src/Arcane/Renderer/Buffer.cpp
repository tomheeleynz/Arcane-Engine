#include "Buffer.h"
#include "Arcane/Renderer/Renderer.h"

// Platform Specific
// -- Vulkan
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanBuffer.h"
#endif

namespace Arcane {

	////////////////////////////////////////////////////////
	//// Vertex Buffer
	////////////////////////////////////////////////////////
	VertexBuffer* VertexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanVertexBuffer(data, size);
#endif
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
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanIndexBuffer(data, count);
#endif
		default: return nullptr;
		}
	}
}

#include "Buffer.h"
#include "Renderer/Renderer.h"

// Platform Specific
// -- Vulkan
#include "Platform/Vulkan/VulkanBuffer.h"

////////////////////////////////////////////////////////
//// Vertex Buffer
////////////////////////////////////////////////////////
VertexBuffer* VertexBuffer::Create(void* data, uint32_t size)
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::Vulkan: return new VulkanVertexBuffer(data, size);
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
	default: return nullptr;
	}
}
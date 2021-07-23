#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/Buffer.h"

namespace Arcane {
	////////////////////////////////////////////////////////
	//// Vertex Buffer
	////////////////////////////////////////////////////////
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(void* data, uint32_t size);

		VkBuffer GetVertexBuffer() { return m_VertexBuffer; }
		void AddIndexBuffer(IndexBuffer* _indexBuffer) { m_IndexBuffer = _indexBuffer; }

		IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }
	private:
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		IndexBuffer* m_IndexBuffer = nullptr;
	};


	////////////////////////////////////////////////////////
	//// Index Buffer
	////////////////////////////////////////////////////////
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(void* data, uint32_t count);

		VkBuffer GetIndexBuffer() { return m_IndexBuffer; }

		uint32_t GetCount() { return m_Count; }
	private:
		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;
		uint32_t m_Count;
	};
}
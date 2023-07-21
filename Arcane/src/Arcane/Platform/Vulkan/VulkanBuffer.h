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
		VulkanVertexBuffer(uint32_t size);

		VkBuffer GetVertexBuffer() { return m_VertexBuffer; }
		void AddIndexBuffer(IndexBuffer* _indexBuffer) { m_IndexBuffer = _indexBuffer; }

		IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }

		void SetData(void* data, uint32_t size) override;
	private:
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	private:
		VkBuffer m_VertexBuffer;
		VkBuffer m_StagingBuffer;

		VkDeviceMemory m_VertexBufferMemory;
		VkDeviceMemory m_StagingBufferMemory;

		IndexBuffer* m_IndexBuffer = nullptr;
	};


	////////////////////////////////////////////////////////
	//// Index Buffer
	////////////////////////////////////////////////////////
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(void* data, uint32_t count);
		VulkanIndexBuffer(uint32_t count);

		VkBuffer GetIndexBuffer() { return m_IndexBuffer; }

		uint32_t GetCount() { return m_Count; }
	private:
		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;
		uint32_t m_Count;
	};
}
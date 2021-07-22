#include "VulkanBuffer.h"
#include "Core/Application.h"
#include "VulkanContext.h"

namespace Arcane {
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		return -1;
	}

	////////////////////////////////////////////////////////
	//// Vertex Buffer
	////////////////////////////////////////////////////////
	VulkanVertexBuffer::VulkanVertexBuffer(void* vertices, uint32_t size)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS) {
			printf("Vertex Buffer Not Created\n");
		}
		else {
			printf("Vertex Buffer Created\n");
		}

		// Get buffer memeory requirements
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_VertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS) {
			printf("Failed to allocated vertex buffer memory\n");
		}
		else {
			printf("Allocated vertex buffer memory\n");
		}

		vkBindBufferMemory(logicalDevice, m_VertexBuffer, m_VertexBufferMemory, 0);

		// Add Data To Vertex Buffer
		void* data;
		vkMapMemory(logicalDevice, m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, vertices, (size_t)bufferInfo.size);
		vkUnmapMemory(logicalDevice, m_VertexBufferMemory);
	}



	////////////////////////////////////////////////////////
	//// Index Buffer
	////////////////////////////////////////////////////////
	VulkanIndexBuffer::VulkanIndexBuffer(void* indices, uint32_t count)
	{
		m_Count = count;

		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = count * sizeof(uint32_t);
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_IndexBuffer) != VK_SUCCESS) {
			printf("Index Buffer Not Created\n");
		}
		else {
			printf("Index Buffer Created\n");
		}

		// Get buffer memeory requirements
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_IndexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_IndexBufferMemory) != VK_SUCCESS) {
			printf("Failed to allocated index buffer memory\n");
		}
		else {
			printf("Allocated index buffer memory\n");
		}

		vkBindBufferMemory(logicalDevice, m_IndexBuffer, m_IndexBufferMemory, 0);

		// Add Data To Vertex Buffer
		void* data;
		vkMapMemory(logicalDevice, m_IndexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, indices, (size_t)bufferInfo.size);
		vkUnmapMemory(logicalDevice, m_IndexBufferMemory);
	}

}
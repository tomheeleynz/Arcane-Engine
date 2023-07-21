#include "VulkanBuffer.h"
#include "Arcane/Core/Application.h"
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

		VkDeviceSize bufferSize = size;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_StagingBuffer, m_StagingBufferMemory);

		void* data;
		vkMapMemory(logicalDevice, m_StagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices, (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, m_StagingBufferMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		CopyBuffer(m_StagingBuffer, m_VertexBuffer, bufferSize);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkDeviceSize bufferSize = size;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_StagingBuffer, m_StagingBufferMemory);
		
		void* data;
		vkMapMemory(logicalDevice, m_StagingBufferMemory, 0, bufferSize, 0, &data);
		// memcpy(data, nullptr, (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, m_StagingBufferMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		CopyBuffer(m_StagingBuffer, m_VertexBuffer, bufferSize);

		//VkBufferCreateInfo bufferInfo{};
		//bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//bufferInfo.size = size;
		//bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		//bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		//if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS) {
		//	printf("Vertex Buffer Not Created\n");
		//}
		//else {
		//	printf("Vertex Buffer Created\n");
		//}

		//// Get buffer memeory requirements
		//VkMemoryRequirements memRequirements;
		//vkGetBufferMemoryRequirements(logicalDevice, m_VertexBuffer, &memRequirements);

		//VkMemoryAllocateInfo allocInfo{};
		//allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//allocInfo.allocationSize = memRequirements.size;
		//allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		//if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS) {
		//	printf("Failed to allocated vertex buffer memory\n");
		//}
		//else {
		//	printf("Allocated vertex buffer memory\n");
		//}

		//vkBindBufferMemory(logicalDevice, m_VertexBuffer, m_VertexBufferMemory, 0);

		//// Add Data To Vertex Buffer
		//void* data;
		//vkMapMemory(logicalDevice, m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
		//memcpy(data, nullptr, (size_t)bufferInfo.size);
		//vkUnmapMemory(logicalDevice, m_VertexBufferMemory);
	}

	void VulkanVertexBuffer::SetData(void* data, uint32_t size)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkDeviceSize bufferSize = size;

		void* vertices;
		vkMapMemory(logicalDevice, m_StagingBufferMemory, 0,bufferSize , 0, &vertices);
		memcpy(vertices, data, (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, m_StagingBufferMemory);

		CopyBuffer(m_StagingBuffer, m_VertexBuffer, bufferSize);
	}

	void VulkanVertexBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			printf("failed to create buffer!\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
	}

	void VulkanVertexBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer);
	}

	VkCommandBuffer VulkanVertexBuffer::BeginSingleTimeCommands()
	{
		Application& app = Application::Get();
		VkCommandPool commandPool = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetSwapChain().GetCommandPool();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanVertexBuffer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		Application& app = Application::Get();
		VkCommandPool commandPool = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetSwapChain().GetCommandPool();
		VulkanDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice();

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;

		VkFence fence;

		if (vkCreateFence(logicalDevice.GetLogicalDevice(), &fenceCreateInfo, nullptr, &fence) != VK_SUCCESS) {
			printf("Single time fence not created\n");
		}

		vkQueueSubmit(logicalDevice.GetGraphicsQueue(), 1, &submitInfo, fence);

		vkWaitForFences(logicalDevice.GetLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

		vkDestroyFence(logicalDevice.GetLogicalDevice(), fence, nullptr);

		vkFreeCommandBuffers(logicalDevice.GetLogicalDevice(), commandPool, 1, &commandBuffer);
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
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
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

	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t count)
	{
		m_Count = count;

		Application& app = Application::Get();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VkPhysicalDevice physicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetPhysicalDevice().GetPhysicalDevice();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = count * sizeof(uint32_t);
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
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
	}

}
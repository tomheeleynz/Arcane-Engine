#include "VulkanUniformBuffer.h"
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

	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size)
	{
		m_Size = size;

		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

		// Create set descriptor layout
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &m_DescriptorLayout) != VK_SUCCESS) {
			printf("Descriptor Set Layout Not Created\n");
		}
		else {
			printf("Descriptor Set Created\n");
		}

		// Create Uniform Buffers
		VkDeviceSize bufferSize = size;

		m_UniformBuffers.resize(context->GetSwapChain().GetSwapChainImagesSize());
		m_UniformBuffersMemory.resize(context->GetSwapChain().GetSwapChainImagesSize());

		for (int i = 0; i < context->GetSwapChain().GetSwapChainImagesSize(); i++) {
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = bufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_UniformBuffers[i]) != VK_SUCCESS) {
				printf("Uniform Buffer Not Created\n");
			}
			else {
				printf("Uniform Buffer Created\n");
			}

			// Get buffer memeory requirements
			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(logicalDevice, m_UniformBuffers[i], &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_UniformBuffersMemory[i]) != VK_SUCCESS) {
				printf("Failed to allocated uniform buffer memory\n");
			}
			else {
				printf("Allocated uniform buffer memory\n");
			}
		}

		// Creating Descriptor Pools
		VkDescriptorPoolSize poolSize{};
		poolSize.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = context->GetSwapChain().GetSwapChainImagesSize();

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = context->GetSwapChain().GetSwapChainImagesSize();

		if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &m_DescriptorPool)) {
			printf("Failed to create descriptor pool\n");
		}
		else {
			printf("Created descriptor pool\n");
		}

		std::vector<VkDescriptorSetLayout> layouts(context->GetSwapChain().GetSwapChainImagesSize(), m_DescriptorLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = context->GetSwapChain().GetSwapChainImagesSize();
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(context->GetSwapChain().GetSwapChainImagesSize());

		if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
			printf("Descriptor Sets Not Allocated\n");
		}

		// Now we populate every descriptor
		for (size_t i = 0; i < context->GetSwapChain().GetSwapChainImagesSize(); i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;

			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void VulkanUniformBuffer::WriteData(void* data)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

		// Copy data into uniform buffer memory
		vkMapMemory(logicalDevice, m_UniformBuffersMemory[context->GetSwapChain().GetImageIndex()], 0, m_Size, 0, &data);
		memcpy(data, data, m_Size);
		vkUnmapMemory(logicalDevice, m_UniformBuffersMemory[context->GetSwapChain().GetImageIndex()]);
	}
}
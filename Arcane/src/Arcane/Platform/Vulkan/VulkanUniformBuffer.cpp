#include "VulkanUniformBuffer.h"
#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"

namespace Arcane
{
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

	VulkanUniformBuffer::VulkanUniformBuffer(std::initializer_list<UniformDescriptor*> descriptors)
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

		uint32_t uniformBufferSize = 0;
		bool foundUniformBuffer = false;

		// Get The sizes for creating uniform buffers
		for (UniformDescriptor* descriptor : descriptors) 
		{
			UniformDescriptorType currentType = descriptor->GetType();

			switch (currentType)
			{
			case UniformDescriptorType::UniformBufferObject: {
				UniformObject* object = static_cast<UniformObject*>(descriptor);
				uniformBufferSize = object->GetSize();
				foundUniformBuffer = true;
				break;
			}
			default:
				break;
			}
		}

		VkDeviceSize bufferSize = uniformBufferSize;
		m_UniformBuffers.resize(swapchain.GetSwapChainImagesSize());
		m_UniformBuffersMemory.resize(swapchain.GetSwapChainImagesSize());

		for (int i = 0; i < swapchain.GetSwapChainImagesSize(); i++) {
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = uniformBufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_UniformBuffers[i]) != VK_SUCCESS) {
				printf("Uniform Buffer Not Created\n");
			}
			else {
				printf("Uniform Buffer Created\n");
			}

			if (foundUniformBuffer) {
				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(logicalDevice, m_UniformBuffers[i], &memRequirements);

				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

				if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_UniformBuffersMemory[i]) != VK_SUCCESS) {
					printf("Failed to allocated Uniform buffer memory\n");
				}
				else {
					printf("Allocated Uniform buffer memory\n");
				}

				vkBindBufferMemory(logicalDevice, m_UniformBuffers[i], m_UniformBuffersMemory[i], 0);
			}
		}

		m_DescriptorSet = new VulkanDescriptorSet(m_UniformBuffers, descriptors);
	}

	void VulkanUniformBuffer::WriteData(void* data, uint32_t size)
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

		void* tempData;
		vkMapMemory(logicalDevice, m_UniformBuffersMemory[swapchain.GetImageIndex()], 0, size, 0, &tempData);
		memcpy(tempData, data,  size);
		vkUnmapMemory(logicalDevice, m_UniformBuffersMemory[swapchain.GetImageIndex()]);
	}

	void VulkanUniformBuffer::WriteData(UniformObject* object)
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

		void* tempData;
		vkMapMemory(logicalDevice, m_UniformBuffersMemory[swapchain.GetImageIndex()], 0, object->GetSize(), 0, &tempData);
		memcpy(tempData, object->GetData(), object->GetSize());
		vkUnmapMemory(logicalDevice, m_UniformBuffersMemory[swapchain.GetImageIndex()]);
	}

}

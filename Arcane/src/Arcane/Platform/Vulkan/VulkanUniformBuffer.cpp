#include "VulkanUniformBuffer.h"
#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"

namespace Arcane
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size)
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();

		// Size of uniform buffer
		VkDeviceSize bufferSize = size;
		m_UniformBuffers.resize(imageCount);
		m_UniformBuffersMemory.resize(imageCount);
		m_Size = size;

		for (int i = 0; i < imageCount; i++) {
			// Create Uniform Buffer
			context->GetDevice().CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBuffersMemory[i]);
		}
	}

	void VulkanUniformBuffer::WriteData(void* data, uint32_t size)
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

		void* tempData;
		vkMapMemory(logicalDevice, m_UniformBuffersMemory[swapchain.GetImageIndex()], 0, size, 0, &tempData);
		memcpy(tempData, data, size);
		vkUnmapMemory(logicalDevice, m_UniformBuffersMemory[swapchain.GetImageIndex()]);
	}

	uint32_t VulkanUniformBuffer::GetSize() 
	{
		return m_Size;
	}
}

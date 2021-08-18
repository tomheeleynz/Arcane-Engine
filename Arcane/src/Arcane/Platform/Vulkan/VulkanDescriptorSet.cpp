#include "VulkanDescriptorSet.h"
#include "VulkanContext.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	VulkanDescriptorSet::VulkanDescriptorSet(std::vector<VkBuffer> uniformBuffers, uint32_t size)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VulkanSwapChain& swapchain = context->GetSwapChain();

		VkDescriptorSetLayoutBinding uboBinding{};
		uboBinding.binding = 0;
		uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboBinding.descriptorCount = 1;
		uboBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		uboBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboBinding;

		if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &m_Layout) != VK_SUCCESS) {
			printf("Uniform Descriptor Layout Not Created\n");
		}

		VkDescriptorPoolSize poolSize{};
		poolSize.descriptorCount = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = swapchain.GetSwapChainImagesSize();

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = swapchain.GetSwapChainImagesSize();

		if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
			printf("Descriptor Pool Not Created\n");
		}

		std::vector<VkDescriptorSetLayout> layouts(swapchain.GetSwapChainImagesSize(), m_Layout);
		
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = swapchain.GetSwapChainImagesSize();
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(swapchain.GetSwapChainImagesSize());
		if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
			printf("Failed to allocate descriptor sets\n");
		}

		for (int i = 0; i < swapchain.GetSwapChainImagesSize(); i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = size;

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
		}
	}
}
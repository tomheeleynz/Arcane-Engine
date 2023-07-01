#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanSet.h"
#include "VulkanUniformBuffer.h"
#include "VulkanTexture.h"
#include "VulkanFramebuffer.h"

namespace Arcane
{
	VulkanSet::VulkanSet(DescriptorSetSpecs& specs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs)
	{
		m_SetNumber = specs.SetNumber;

		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();

		// Iterate through Layouts, create bindings for each
		// Manage the counts of each set
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		for (DescriptorLayoutSpecs spec : layoutSpecs) {
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = spec.Binding;
			binding.descriptorCount = spec.DescriptorCount;
			binding.pImmutableSamplers = nullptr;

			switch (spec.Location)
			{
			case DescriptorLocation::VERTEX:
				binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case DescriptorLocation::FRAGMENT:
				binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			default:
				break;
			}
			
			// Switch on the type
			switch (spec.Type)
			{
			case DescriptorType::UNIFORM_BUFFER: 
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case DescriptorType::SAMPLER:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			default:
				break;
			}

			bindings.push_back(binding);
		}

		// Actually Create Descriptor set
		VkDescriptorSetLayoutCreateInfo setInfo = {};
		setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setInfo.pNext = nullptr;
		setInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		setInfo.pBindings = bindings.data();
		
		if (vkCreateDescriptorSetLayout(logicalDevice, &setInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
			printf("Uniform Descriptor Layout Not Created\n");
		}

		// Get Descriptor Pool from context
		VkDescriptorPool descriptorPool = context->GetPool();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();
		m_DescriptorSets.resize(imageCount);
		for (int i = 0; i < imageCount; i++) {
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_DescriptorSetLayout;

			if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &m_DescriptorSets[i]) != VK_SUCCESS) {
				printf("Failed to allocate descripor set\n");
			}
		}
	}

	VulkanSet::VulkanSet(DescriptorSetSpecs& specs, std::vector<DescriptorLayoutSpecs> layoutSpecs)
	{
		m_SetNumber = specs.SetNumber;

		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();

		// Iterate through Layouts, create bindings for each
		// Manage the counts of each set
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		for (DescriptorLayoutSpecs spec : layoutSpecs) {
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = spec.Binding;
			binding.descriptorCount = spec.DescriptorCount;
			binding.pImmutableSamplers = nullptr;

			switch (spec.Location)
			{
			case DescriptorLocation::VERTEX:
				binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case DescriptorLocation::FRAGMENT:
				binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			default:
				break;
			}

			// Switch on the type
			switch (spec.Type)
			{
			case DescriptorType::UNIFORM_BUFFER:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case DescriptorType::SAMPLER:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			default:
				break;
			}

			bindings.push_back(binding);
		}

		// Actually Create Descriptor set
		VkDescriptorSetLayoutCreateInfo setInfo = {};
		setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setInfo.pNext = nullptr;
		setInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		setInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(logicalDevice, &setInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
			printf("Uniform Descriptor Layout Not Created\n");
		}

		// Get Descriptor Pool from context
		VkDescriptorPool descriptorPool = context->GetPool();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();
		m_DescriptorSets.resize(imageCount);
		for (int i = 0; i < imageCount; i++) {
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_DescriptorSetLayout;

			if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &m_DescriptorSets[i]) != VK_SUCCESS) {
				printf("Failed to allocate descripor set\n");
			}
		}
	}

	void VulkanSet::AddUniformBuffer(UniformBuffer* buffer, uint32_t setNumber, uint32_t bindingNumber)
	{
		// Get Image Count
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();
		VulkanUniformBuffer* vulkanBuffer = static_cast<VulkanUniformBuffer*>(buffer);

		// Get Uniform Buffers
		std::vector<VkBuffer> buffers = vulkanBuffer->GetUniformBuffers();

		for (int i = 0; i < imageCount; i++) {
			// Write to descriptor sets
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = buffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = vulkanBuffer->GetSize();
			
			// Create a descriptor write, then update descriptor set
			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = bindingNumber;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			// Update Descriptor Sets
			vkUpdateDescriptorSets(context->GetDevice().GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}

	void VulkanSet::AddImageSampler(Texture* texture, uint32_t setNumber, uint32_t bindingNumber)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();

		VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture);
		
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = vulkanTexture->GetImageDescriptorInfo().imageView;
		imageInfo.sampler = vulkanTexture->GetImageDescriptorInfo().sampler;

		for (int i = 0; i < imageCount; i++) {
			// Create a descriptor write, then update descriptor set
			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = bindingNumber;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;

			// Update Descriptor Sets
			vkUpdateDescriptorSets(context->GetDevice().GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}

	void VulkanSet::AddImageSampler(Framebuffer* framebuffer, uint32_t setNumber, uint32_t bindingNumber)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();

		VulkanFramebuffer* vulkanFamebuffer = static_cast<VulkanFramebuffer*>(framebuffer);
	
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = static_cast<VulkanColorAttachment*>(vulkanFamebuffer->GetColorAttachment(0))->imageInfo.imageView;
		imageInfo.sampler = static_cast<VulkanColorAttachment*>(vulkanFamebuffer->GetColorAttachment(0))->imageSampler;

		for (int i = 0; i < imageCount; i++) {
			// Create a descriptor write, then update descriptor set
			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = bindingNumber;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;

			// Update Descriptor Sets
			vkUpdateDescriptorSets(context->GetDevice().GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}
}
#include "VulkanDescriptorSet.h"
#include "VulkanContext.h"
#include "VulkanFramebuffer.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	VulkanDescriptorSet::VulkanDescriptorSet(std::vector<VkBuffer> uniformBuffers, std::initializer_list<UniformDescriptor*> descriptors)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VulkanSwapChain& swapchain = context->GetSwapChain();
		
		uint32_t bindingCount = 0;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		for (UniformDescriptor* descriptor : descriptors) 
		{
			UniformDescriptorType currentType = descriptor->GetType();

			switch (currentType)
			{
			case UniformDescriptorType::UniformBufferObject: 
			{
				VkDescriptorSetLayoutBinding uboBinding{};
				uboBinding.binding = bindingCount;
				uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboBinding.descriptorCount = 1;
				uboBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				uboBinding.pImmutableSamplers = nullptr;

				layoutBindings.push_back(uboBinding);
				break;
			}
			case UniformDescriptorType::TextureSampler:
			{
				VkDescriptorSetLayoutBinding samplerLayoutBinding{};
				samplerLayoutBinding.binding = bindingCount;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr;
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				layoutBindings.push_back(samplerLayoutBinding);
				break;
			}
			default:
				break;
			}
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &m_Layout) != VK_SUCCESS) {
			printf("Uniform Descriptor Layout Not Created\n");
		}


		std::vector<VkDescriptorPoolSize> poolSizes;
		for (UniformDescriptor* descriptor : descriptors)
		{
			UniformDescriptorType currentType = descriptor->GetType();

			switch (currentType)
			{
			case UniformDescriptorType::UniformBufferObject:
			{
				VkDescriptorPoolSize poolSize;
				poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				poolSize.descriptorCount = static_cast<uint32_t>(swapchain.GetSwapChainImagesSize());
				poolSizes.push_back(poolSize);
				break;
			}
			case UniformDescriptorType::TextureSampler:
			{
				VkDescriptorPoolSize poolSize;
				poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSize.descriptorCount = static_cast<uint32_t>(swapchain.GetSwapChainImagesSize());
				poolSizes.push_back(poolSize);
				break;
			}
			default:
				break;
			}
		}

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchain.GetSwapChainImagesSize());

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

		for (size_t i = 0; i < swapchain.GetSwapChainImagesSize(); i++) {

 			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.resize(descriptors.size());

			int bindingCount = 0;

			for (UniformDescriptor* descriptor : descriptors) {
				UniformDescriptorType currentType = descriptor->GetType();

				switch (currentType)
				{
				case UniformDescriptorType::UniformBufferObject:
				{
					UniformObject* uniformObject = static_cast<UniformObject*>(descriptor);
					
					VkDescriptorBufferInfo bufferInfo{};
					bufferInfo.buffer = uniformBuffers[i];
					bufferInfo.offset = 0;
					bufferInfo.range = uniformObject->GetSize();
					
					descriptorWrites[bindingCount].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[bindingCount].dstSet = m_DescriptorSets[i];
					descriptorWrites[bindingCount].dstBinding = descriptor->GetBinding();
					descriptorWrites[bindingCount].dstArrayElement = 0;
					descriptorWrites[bindingCount].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorWrites[bindingCount].descriptorCount = 1;
					descriptorWrites[bindingCount].pBufferInfo = &bufferInfo;
					
					bindingCount++;
					break;
				}
				case UniformDescriptorType::TextureSampler:
				{
					TextureSampler* textureSampler = static_cast<TextureSampler*>(descriptor);
					VkDescriptorImageInfo imageInfo;

					if (textureSampler->GetType() == TextureSampler::TextureType::BASIC) {
						VulkanTexture* texture = static_cast<VulkanTexture*>(textureSampler->GetTexture());
						imageInfo = texture->GetImageDescriptorInfo();
					}
					else {
						VulkanFramebuffer* framebuffer = static_cast<VulkanFramebuffer*>(textureSampler->GetFramebuffer());
						imageInfo = framebuffer->GetDescriptor();
					}

					descriptorWrites[bindingCount].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[bindingCount].dstSet = m_DescriptorSets[i];
					descriptorWrites[bindingCount].dstBinding = descriptor->GetBinding();
					descriptorWrites[bindingCount].dstArrayElement = 0;
					descriptorWrites[bindingCount].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrites[bindingCount].descriptorCount = 1;
					descriptorWrites[bindingCount].pImageInfo = &imageInfo;
					bindingCount++;
					break;
				}
				default:
					break;
				}
			}

			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

	}

	VulkanDescriptorSet::VulkanDescriptorSet(std::initializer_list<UniformDescriptor*> descriptors)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();
		VulkanSwapChain& swapchain = context->GetSwapChain();

		uint32_t bindingCount = 0;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		for (UniformDescriptor* descriptor : descriptors)
		{
			UniformDescriptorType currentType = descriptor->GetType();

			switch (currentType)
			{
			case UniformDescriptorType::UniformBufferObject:
			{
				VkDescriptorSetLayoutBinding uboBinding{};
				uboBinding.binding = bindingCount;
				uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboBinding.descriptorCount = 1;
				uboBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				uboBinding.pImmutableSamplers = nullptr;

				layoutBindings.push_back(uboBinding);
				break;
			}
			case UniformDescriptorType::TextureSampler:
			{
				VkDescriptorSetLayoutBinding samplerLayoutBinding{};
				samplerLayoutBinding.binding = bindingCount;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr;
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				layoutBindings.push_back(samplerLayoutBinding);
				break;
			}
			default:
				break;
			}
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &m_Layout) != VK_SUCCESS) {
			printf("Uniform Descriptor Layout Not Created\n");
		}


		std::vector<VkDescriptorPoolSize> poolSizes;
		for (UniformDescriptor* descriptor : descriptors)
		{
			UniformDescriptorType currentType = descriptor->GetType();

			switch (currentType)
			{
			case UniformDescriptorType::UniformBufferObject:
			{
				VkDescriptorPoolSize poolSize;
				poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				poolSize.descriptorCount = static_cast<uint32_t>(swapchain.GetSwapChainImagesSize());
				poolSizes.push_back(poolSize);
				break;
			}
			case UniformDescriptorType::TextureSampler:
			{
				VkDescriptorPoolSize poolSize;
				poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSize.descriptorCount = static_cast<uint32_t>(swapchain.GetSwapChainImagesSize());
				poolSizes.push_back(poolSize);
				break;
			}
			default:
				break;
			}
		}

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchain.GetSwapChainImagesSize());

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

		for (size_t i = 0; i < swapchain.GetSwapChainImagesSize(); i++) {

			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.resize(descriptors.size());

			int bindingCount = 0;

			for (UniformDescriptor* descriptor : descriptors) {
				UniformDescriptorType currentType = descriptor->GetType();

				switch (currentType)
				{
				case UniformDescriptorType::UniformBufferObject:
				{
					break;
				}
				case UniformDescriptorType::TextureSampler:
				{
					TextureSampler* textureSampler = static_cast<TextureSampler*>(descriptor);
					VkDescriptorImageInfo imageInfo;

					if (textureSampler->GetType() == TextureSampler::TextureType::BASIC) {
						VulkanTexture* texture = static_cast<VulkanTexture*>(textureSampler->GetTexture());
						imageInfo = texture->GetImageDescriptorInfo();
					}
					else {
						VulkanFramebuffer* framebuffer = static_cast<VulkanFramebuffer*>(textureSampler->GetFramebuffer());
						imageInfo = framebuffer->GetDescriptor();
					}

					descriptorWrites[bindingCount].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[bindingCount].dstSet = m_DescriptorSets[i];
					descriptorWrites[bindingCount].dstBinding = descriptor->GetBinding();
					descriptorWrites[bindingCount].dstArrayElement = 0;
					descriptorWrites[bindingCount].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrites[bindingCount].descriptorCount = 1;
					descriptorWrites[bindingCount].pImageInfo = &imageInfo;
					bindingCount++;
					break;
				}
				default:
					break;
				}
			}

			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

	}
}
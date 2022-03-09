#include "VulkanFramebuffer.h"

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

	VulkanFramebuffer::VulkanFramebuffer(FramebufferSpecifications& specs)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(app.GetWindow().GetContext());

		m_Specs = specs;

		for (FramebufferAttachmentType element : specs.AttachmentSpecs.m_Attachments) {
			switch (element) {
				case FramebufferAttachmentType::COLOR:
				{
					FrameBufferAttachment attachment = {};

					// Image Create Info
					VkImageCreateInfo imageCreateInfo = {};
					imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
					imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
					imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
					imageCreateInfo.extent.width = specs.Width;
					imageCreateInfo.extent.height = specs.Height;
					imageCreateInfo.extent.depth = 1;
					imageCreateInfo.mipLevels = 1;
					imageCreateInfo.arrayLayers = 1;
					imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
					imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

					// Create Image color attachment
					if (vkCreateImage(context->GetDevice().GetLogicalDevice(), &imageCreateInfo, nullptr, &attachment.Image) != VK_SUCCESS) {
						printf("Framebuffer Color not created\n");
					}

					// Get Image Memory Requirmentes
					VkMemoryRequirements memRequirements;
					vkGetImageMemoryRequirements(context->GetDevice().GetLogicalDevice(), attachment.Image, &memRequirements);

					VkMemoryAllocateInfo allocInfo{};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

					if (vkAllocateMemory(context->GetDevice().GetLogicalDevice(), &allocInfo, nullptr, &attachment.ImageMemory) != VK_SUCCESS) {
						printf("Framebuffer Color Image Memory Not Allocated\n");
					}


					if (vkBindImageMemory(context->GetDevice().GetLogicalDevice(), attachment.Image, attachment.ImageMemory, 0) != VK_SUCCESS) {
						printf("Framebuffer Color Image Memory Not Bound\n");
					}

					TransitionImageLayout(attachment.Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
					TransitionImageLayout(attachment.Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

					// Create Image View
					VkImageViewCreateInfo colorImageInfo = {};
					colorImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					colorImageInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
					colorImageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
					colorImageInfo.subresourceRange = {};
					colorImageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					colorImageInfo.subresourceRange.baseMipLevel = 0;
					colorImageInfo.subresourceRange.levelCount = 1;
					colorImageInfo.subresourceRange.baseArrayLayer = 0;
					colorImageInfo.subresourceRange.layerCount = 1;
					colorImageInfo.image = attachment.Image;

					if (vkCreateImageView(context->GetDevice().GetLogicalDevice(), &colorImageInfo, nullptr, &attachment.ImageView) != VK_SUCCESS)
					{
						printf("Framebuffer Color Image View Not Created\n");
					}

					// Create Image 
					VkSamplerCreateInfo samplerInfo{};
					samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
					samplerInfo.magFilter = VK_FILTER_LINEAR;
					samplerInfo.minFilter = VK_FILTER_LINEAR;
					samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
					samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
					samplerInfo.addressModeV = samplerInfo.addressModeU;
					samplerInfo.addressModeW = samplerInfo.addressModeU;
					samplerInfo.mipLodBias = 0.0f;
					samplerInfo.maxAnisotropy = 1.0f;
					samplerInfo.minLod = 0.0f;
					samplerInfo.maxLod = 1.0f;
					samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

					if (vkCreateSampler(context->GetDevice().GetLogicalDevice(), &samplerInfo, nullptr, &m_ImageSampler) != VK_SUCCESS)
					{
						printf("Color Sampler Not Created in framebuffer\n");
					}

					m_Attachments.push_back(attachment);

					break;
				}
			}

			// Create Framebuffer Renderpass
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			// Subpass Dependency
			VkSubpassDependency dependency{};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;

			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;

			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			if (vkCreateRenderPass(context->GetDevice().GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
				printf("Framebuffer Renderpass not created\n");
			}
			else {
				printf("Framebuffer Render Pass created\n");
			}

			VkImageView attachments[1];
			attachments[0] = m_Attachments[0].ImageView;

			VkFramebufferCreateInfo fbCreateInfo{};
			fbCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			fbCreateInfo.renderPass = m_RenderPass;
			fbCreateInfo.pAttachments = attachments;
			fbCreateInfo.attachmentCount = 1;
			fbCreateInfo.width = specs.Width;
			fbCreateInfo.height = specs.Height;
			fbCreateInfo.layers = 1;

			if (vkCreateFramebuffer(context->GetDevice().GetLogicalDevice(), &fbCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS) {
				printf("Framebuffer  not created\n");
			}
			else {
				printf("Framebuffer  created\n");
			}

			m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_ImageDescriptor.imageView = m_Attachments[0].ImageView;
			m_ImageDescriptor.sampler = m_ImageSampler;
		}
	}

	void VulkanFramebuffer::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		
		Application& app = Application::Get();
		VulkanDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice();
		VkCommandBuffer commandBuffer = logicalDevice.CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		VkImageMemoryBarrier barrier{};

		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			printf("unsupported layout transition!\n");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		logicalDevice.FlushCommandBuffer(commandBuffer);
	}
}

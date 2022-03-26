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
		TestSpecs = &specs;

		Create();
	}

	void VulkanFramebuffer::Create()
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(app.GetWindow().GetContext());

		uint32_t attachmentCount = 0;
		
		std::vector<VkImageView> attachments;
		attachments.resize(m_Specs.AttachmentSpecs.m_Attachments.size());

		for (FramebufferAttachmentType element : m_Specs.AttachmentSpecs.m_Attachments) {
			switch (element) {
				case FramebufferAttachmentType::COLOR:
				{
					FrameBufferAttachment attachment = {};

					// Image Create Info
					VkImageCreateInfo imageCreateInfo = {};
					imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
					imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
					imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
					imageCreateInfo.extent.width = m_Specs.Width;
					imageCreateInfo.extent.height = m_Specs.Height;
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

					// Create Framebuffer Attachment
					m_Attachments.push_back(attachment);

					// Create Attachment Description
					VkAttachmentDescription colorAttachment{};
					colorAttachment.format = VK_FORMAT_R8G8B8A8_SRGB;
					colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
					colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
					colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					// Create Attachment Reference
					VkAttachmentReference colorAttachmentRef{};
					colorAttachmentRef.attachment = attachmentCount;
					colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

					m_AttachmentsMap.insert({ FramebufferAttachmentType::COLOR, attachment });
					m_AttachmentDescriptionMap.insert({FramebufferAttachmentType::COLOR, colorAttachment});
					m_AttachmentReferenceMap.insert({FramebufferAttachmentType::COLOR, colorAttachmentRef});
					
					attachments[attachmentCount] = attachment.ImageView;

					attachmentCount++;

					m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					m_ImageDescriptor.imageView = attachment.ImageView;
					m_ImageDescriptor.sampler = m_ImageSampler;
				
					break;
				}
				case FramebufferAttachmentType::DEPTH: 
				{
					FrameBufferAttachment attachment = {};

					VkImageCreateInfo image = {};
					image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
					image.imageType = VK_IMAGE_TYPE_2D;
					image.format = VK_FORMAT_D32_SFLOAT;
					image.extent = { m_Specs.Width, m_Specs.Height, 1 };
					image.mipLevels = 1;
					image.arrayLayers = 1;
					image.samples = VK_SAMPLE_COUNT_1_BIT;
					image.tiling = VK_IMAGE_TILING_OPTIMAL;
					image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
					image.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

					if (vkCreateImage(context->GetDevice().GetLogicalDevice(), &image, nullptr, &attachment.Image) != VK_SUCCESS) {
						printf("Framebuffer Depth not created\n");
					}

					// Get Image Memory Requirmentes
					VkMemoryRequirements memRequirements;
					vkGetImageMemoryRequirements(context->GetDevice().GetLogicalDevice(), attachment.Image, &memRequirements);

					VkMemoryAllocateInfo allocInfo{};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

					if (vkAllocateMemory(context->GetDevice().GetLogicalDevice(), &allocInfo, nullptr, &attachment.ImageMemory) != VK_SUCCESS) {
						printf("Framebuffer Depth Image Memory Not Allocated\n");
					}


					if (vkBindImageMemory(context->GetDevice().GetLogicalDevice(), attachment.Image, attachment.ImageMemory, 0) != VK_SUCCESS) {
						printf("Framebuffer Depth Image Memory Not Bound\n");
					}

					VkImageViewCreateInfo depthStencilView = {};
					depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
					depthStencilView.format = VK_FORMAT_D32_SFLOAT;
					depthStencilView.subresourceRange = {};
					depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
					depthStencilView.subresourceRange.baseMipLevel = 0;
					depthStencilView.subresourceRange.levelCount = 1;
					depthStencilView.subresourceRange.baseArrayLayer = 0;
					depthStencilView.subresourceRange.layerCount = 1;
					depthStencilView.image = attachment.Image;


					if (vkCreateImageView(context->GetDevice().GetLogicalDevice(), &depthStencilView, nullptr, &attachment.ImageView) != VK_SUCCESS)
					{
						printf("Framebuffer Depth Image View Not Created\n");
					}

					// Create Attachment Description
					VkAttachmentDescription depthAttachment = {};
					depthAttachment.format = VK_FORMAT_D32_SFLOAT;
					depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
					depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                         
					depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                     
					depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                
					depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;              
					depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                      
					depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

					// Create Attachment Reference
					VkAttachmentReference depthReference;
					depthReference.attachment = attachmentCount;                                            
					depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


					m_AttachmentsMap.insert({FramebufferAttachmentType::DEPTH, attachment});
					m_AttachmentDescriptionMap.insert({ FramebufferAttachmentType::DEPTH, depthAttachment });
					m_AttachmentReferenceMap.insert({ FramebufferAttachmentType::DEPTH, depthReference });

					attachments[attachmentCount] = attachment.ImageView;

					attachmentCount++;

					break;
				}
			}	
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		for (auto const& [key, val] : m_AttachmentReferenceMap) {
			if (key == FramebufferAttachmentType::COLOR) {
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &val;
			}
			else if (key == FramebufferAttachmentType::DEPTH) {
				subpass.pDepthStencilAttachment = &val;
			}
		}

		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		std::vector<VkAttachmentDescription> attachmentDescriptions;
		for (auto const& [key, val] : m_AttachmentDescriptionMap) {
			attachmentDescriptions.push_back(val);
		}

		renderPassInfo.attachmentCount = static_cast<uint32_t>(m_AttachmentDescriptionMap.size());
		renderPassInfo.pAttachments = attachmentDescriptions.data();

		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		if (vkCreateRenderPass(context->GetDevice().GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
			printf("Framebuffer Renderpass not created\n");
		}
		else {
			printf("Framebuffer Render Pass created\n");
		}

		VkFramebufferCreateInfo fbCreateInfo{};
		fbCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbCreateInfo.renderPass = m_RenderPass;
		fbCreateInfo.pAttachments = attachments.data();
		fbCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		fbCreateInfo.width = m_Specs.Width;
		fbCreateInfo.height = m_Specs.Height;
		fbCreateInfo.layers = 1;

		if (vkCreateFramebuffer(context->GetDevice().GetLogicalDevice(), &fbCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS) {
			printf("Framebuffer  not created\n");
		}
		else {
			printf("Framebuffer  created\n");
		}
	}

	void VulkanFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
		VkDevice device = context->GetDevice().GetLogicalDevice();

		m_Specs.Height = height;
		m_Specs.Width = width;
		m_Specs.AttachmentSpecs.m_Attachments = {FramebufferAttachmentType::COLOR, FramebufferAttachmentType::DEPTH};

		// Create new framebuffer
		Create();
	}
}

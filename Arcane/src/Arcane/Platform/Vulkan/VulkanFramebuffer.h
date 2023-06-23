#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <map>


#include "Arcane/Renderer/Framebuffer.h"

namespace Arcane {
	struct VulkanColorAttachment 
	{
		VkDescriptorImageInfo imageInfo;
		VkSampler imageSampler;
	};


	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(FramebufferSpecifications& specs);
		
		VkFramebuffer GetVulkanFramebuffer() { return m_Framebuffer; }

		VkRenderPass GetFramebufferRenderPass() { return m_RenderPass; }

		VkDescriptorImageInfo GetDescriptor() { return m_ImageDescriptor; }

		FramebufferSpecifications& GetSpecs() override { return m_Specs; }

		void Resize(uint32_t width, uint32_t height) override;


		void* GetColorAttachment(uint32_t index = 0) override;
	private:
		void Create();

	private:
		struct FrameBufferAttachment
		{
			VkImage Image;
			VkDeviceMemory ImageMemory;
			VkImageView ImageView;
		};

		std::map<FramebufferAttachmentType, FrameBufferAttachment> m_AttachmentsMap;
		std::map<FramebufferAttachmentType, VkAttachmentDescription> m_AttachmentDescriptionMap;
		std::map<FramebufferAttachmentType, VkAttachmentReference> m_AttachmentReferenceMap;
		
		std::vector<FrameBufferAttachment> m_Attachments;

		FramebufferSpecifications m_Specs;
		FramebufferSpecifications* TestSpecs;

		uint32_t m_Width;
		uint32_t m_Height;

		VkRenderPass m_RenderPass;
		VkFramebuffer m_Framebuffer;

		std::vector<VulkanColorAttachment> m_ColorAttachments;
		VkDescriptorImageInfo m_ImageDescriptor;
		VkSampler m_ImageSampler;
	};
}
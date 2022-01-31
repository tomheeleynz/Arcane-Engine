#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Arcane/Renderer/Framebuffer.h"

namespace Arcane {
	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(FramebufferSpecifications& specs);
		
		VkFramebuffer GetVulkanFramebuffer() { return m_Framebuffer; }

		VkRenderPass GetFramebufferRenderPass() { return m_RenderPass; }

		VkDescriptorImageInfo GetDescriptor() { return m_ImageDescriptor; }


		FramebufferSpecifications GetSpecs() override { return m_Specs; }
	private:
		struct FrameBufferAttachment
		{
			VkImage Image;
			VkDeviceMemory ImageMemory;
			VkImageView ImageView;
		};

		std::vector<FrameBufferAttachment> m_Attachments;

		FramebufferSpecifications m_Specs;

		uint32_t m_Width;
		uint32_t m_Height;

		VkRenderPass m_RenderPass;
		VkFramebuffer m_Framebuffer;
		VkDescriptorImageInfo m_ImageDescriptor;
		VkSampler m_ImageSampler;
	};
}
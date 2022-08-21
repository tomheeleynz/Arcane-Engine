#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/Texture.h"


namespace Arcane 
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(std::string fileName);
		VulkanTexture(float r, float g, float b, float a);

		virtual void UpdateTexture(int data) override;
		virtual void UpdateTexture(std::string filename) override;
		virtual void UpdateTexture(Texture* texture) override;


		VkDescriptorImageInfo GetImageDescriptorInfo() { return m_ImageInfo; }
	private:
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		VkImageView CreateImageView(VkImage image, VkFormat format);
		void CreateTextureImageView();
		void CreateTextureSampler();
	private:
		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
		VkImageView m_TextureImageView;
		VkSampler m_TextureSampler;
		VkDescriptorImageInfo m_ImageInfo;
	};
}
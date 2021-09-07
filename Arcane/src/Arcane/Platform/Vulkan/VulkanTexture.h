#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/Texture.h"


namespace Arcane 
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(std::string fileName);

	private:
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	private:
		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
	};
}
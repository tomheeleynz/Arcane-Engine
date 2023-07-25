#pragma once

#include <vulkan/vulkan.h>
namespace Arcane
{
	class VulkanTools
	{
	public:
		static VkCommandBuffer BeginSingleTimeCommands();
		static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void TransitionImageLayout(VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange);
	};
}
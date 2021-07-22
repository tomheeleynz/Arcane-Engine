#pragma once

#include <algorithm>
#include <array>
#include "VulkanDevice.h"

namespace Arcane {

	class VulkanSwapChain
	{
	public:
		VulkanSwapChain(VulkanDevice& _device, VkSurfaceKHR _surface);

		VkExtent2D GetExtent() { return m_Extent; }
		VkFormat GetFormat() { return m_Format; }
		VkPresentModeKHR GetPresentMode() { return m_PresentMode; }

		std::vector<VkCommandBuffer> GetCommandBuffers() { return m_CommandBuffers; }
		std::vector<VkFramebuffer> GetSwapChainFramebuffers() { return m_SwapChainFramebuffers; }
		uint32_t GetSwapChainImagesSize() { return m_SwapChainImages.size(); }

		void SwapBuffers();
	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkSwapchainKHR m_SwapChain;

		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		VkExtent2D m_Extent;
		VkFormat m_Format;
		VkPresentModeKHR m_PresentMode;

		VkRenderPass m_RenderPass;
		VkCommandPool m_CommandPool;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;

		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		int m_MaxFramesInFlight = 2;
		size_t m_CurrentFrameIndex = 0;
	};
}
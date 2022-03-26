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
		uint32_t GetImageIndex() { return m_CurrentBuffer; }
		uint32_t GetFrameIndex() { return m_CurrentFrameIndex; }
		uint32_t GetMaxFranesInFlight() { return m_MaxFramesInFlight; }

		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkRenderPass GetSwapchainRenderPass() { return m_RenderPass; }

		void SwapBuffers();

		// Get Semaphores and Stuff
		VkSemaphore GetCurrentImageSemaphore() { return m_ImageAvailableSemaphores[m_CurrentFrameIndex]; }
		VkSemaphore GetCurrentRenderSemaphore() { return m_RenderFinishedSemaphores[m_CurrentFrameIndex]; };
		
		// Get Current Framebuffer
		VkFramebuffer GetCurrentFramebuffer() { return m_SwapChainFramebuffers[m_CurrentFrameIndex]; }

		// Get Native Swapchain
		VkSwapchainKHR GetSwapchain() { return m_SwapChain; }

		// Get Current Draw Command Buffer
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return m_CommandBuffers[m_CurrentFrameIndex]; }

		// Recreate Swapchain

		// Aquire Swapchain Image
		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);

		// Present Queue
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore);
	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void RecreateSwapchain();

		// Creation Functions
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreateCommandPool() ;
		void CreateCommandBuffers();
	private:
		VkSwapchainKHR m_SwapChain;
		uint32_t m_ImageCount = -1;

		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		VkExtent2D m_Extent;
		VkFormat m_Format;
		VkPresentModeKHR m_PresentMode;

		VkRenderPass m_RenderPass;
		VkCommandPool m_CommandPool;

		// Image Semaphores
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;

		// InFlight Images
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		int m_MaxFramesInFlight = 2;
		size_t m_CurrentFrameIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		// Information Passed from vulkan context
		VulkanDevice* m_VulkanDevice;
		VkSurfaceKHR  m_Surface;

		// Changing Swapchain Layout
		VkSemaphore m_PresentCompleteSemaphore;
		VkSemaphore m_RenderCompleteSemaphore;
		std::vector<VkFence> m_WaitFences;
		uint32_t m_CurrentBuffer;
	};
}
#pragma once

#include <vulkan/vulkan.h> 

#include "Arcane/Renderer/GraphicsContext.h"

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

namespace Arcane {
	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();

		void SwapBuffers() override;

		static VkInstance GetInstance() { return m_Instance; };

		VulkanDevice& GetDevice() { return *m_Device; }
		VulkanPhysicalDevice& GetPhysicalDevice() { return *m_PhysicalDevice; }
		VulkanSwapChain& GetSwapChain() { return *m_SwapChain; }
		VkSurfaceKHR GetSurface() { return m_Surface; }
		VkDescriptorPool GetPool() { return m_DescriptorPool; }
	private:
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void SetUpDebugMessenger();
	private:
		inline static VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VulkanPhysicalDevice* m_PhysicalDevice;
		VulkanDevice* m_Device;
		VulkanSwapChain* m_SwapChain;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkDescriptorPool m_DescriptorPool;
	};
}
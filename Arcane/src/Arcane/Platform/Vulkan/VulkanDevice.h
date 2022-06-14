#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <set>
#include <string>

namespace Arcane {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	//////////////////////////////////////////////////////////////////////////////
	//// Vulkan Physical Device
	//////////////////////////////////////////////////////////////////////////////
	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice() = default;
		VulkanPhysicalDevice(VkSurfaceKHR _surface);

		VkPhysicalDevice GetPhysicalDevice();
		QueueFamilyIndices& GetIndices();
		SwapChainSupportDetails& GetSwapChainDetails() { return m_SwapChainDetails; }
		SwapChainSupportDetails& QuerySupportDetails(VkSurfaceKHR _surface);
	private:
		VkPhysicalDevice m_PhysicalDevice;
		QueueFamilyIndices m_Indices;
		SwapChainSupportDetails m_SwapChainDetails;
	};

	//////////////////////////////////////////////////////////////////////////////
	//// Vulkan Logical Device
	//////////////////////////////////////////////////////////////////////////////
	class VulkanDevice
	{
	public:
		VulkanDevice() = default;
		VulkanDevice(VulkanPhysicalDevice& _physicalDevice);

		VulkanPhysicalDevice& GetVulkanPhysicalDevice();
		VkDevice& GetLogicalDevice();

		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; };
		VkQueue GetPresentQueue() { return m_PresentQueue; }

		VkCommandBuffer CreateCommandBuffer(VkCommandBufferLevel commandBufferLevel);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
	
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		VulkanPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;

		// Device Queues
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
	};
}
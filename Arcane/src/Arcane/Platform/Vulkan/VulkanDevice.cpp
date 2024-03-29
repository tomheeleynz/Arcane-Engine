#include "Arcane/Core/Application.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"

namespace Arcane {

	//////////////////////////////////////////////////////////////////////////////
	//// Extensions
	//////////////////////////////////////////////////////////////////////////////
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	//////////////////////////////////////////////////////////////////////////////
	//// Vulkan Physical Device
	//////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice::VulkanPhysicalDevice(VkSurfaceKHR _surface)
	{
		// Create Physical Device
		uint32_t deviceCount = 0;
		VkInstance vulkanInstance = VulkanContext::GetInstance();
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			printf("Not Devices Found\n");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

		m_PhysicalDevice = devices[0];

		// Get Queue Family Indices
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_Indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, _surface, &presentSupport);

			if (presentSupport) {
				m_Indices.presentFamily = i;
			}

			if (m_Indices.IsComplete())
				break;

			i++;
		}

		// Get Swapchain Support Details
		// -- Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, _surface, &m_SwapChainDetails.capabilities);

		// -- Formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, _surface, &formatCount, nullptr);

		if (formatCount != 0) {
			m_SwapChainDetails.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, _surface, &formatCount, m_SwapChainDetails.formats.data());
		}

		// -- Present Modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			m_SwapChainDetails.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, _surface, &presentModeCount, m_SwapChainDetails.presentModes.data());
		}
	}

	SwapChainSupportDetails& VulkanPhysicalDevice::QuerySupportDetails(VkSurfaceKHR _surface)
	{
		// Get Swapchain Support Details
		// -- Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, _surface, &m_SwapChainDetails.capabilities);

		// -- Formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, _surface, &formatCount, nullptr);

		if (formatCount != 0) {
			m_SwapChainDetails.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, _surface, &formatCount, m_SwapChainDetails.formats.data());
		}

		// -- Present Modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			m_SwapChainDetails.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, _surface, &presentModeCount, m_SwapChainDetails.presentModes.data());
		}

		return m_SwapChainDetails;
	}

	VkPhysicalDevice VulkanPhysicalDevice::GetPhysicalDevice()
	{
		return m_PhysicalDevice;
	}

	QueueFamilyIndices& VulkanPhysicalDevice::GetIndices()
	{
		return m_Indices;
	}

	//////////////////////////////////////////////////////////////////////////////
	//// Vulkan Logical Device
	//////////////////////////////////////////////////////////////////////////////
	VulkanDevice::VulkanDevice(VulkanPhysicalDevice& _physicalDevice)
	{
		m_PhysicalDevice = _physicalDevice;

		QueueFamilyIndices& indices = _physicalDevice.GetIndices();
		VkPhysicalDevice physicalDevice = _physicalDevice.GetPhysicalDevice();

		// Extension Loading
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}


		// Device Create Infos
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Create Logical Device
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.enabledLayerCount = 0;

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
			printf("Logical Device Not Created\n");
		}
		else {
			printf("Logical Device Created\n");
		}

		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
	}

	VulkanPhysicalDevice& VulkanDevice::GetVulkanPhysicalDevice()
	{
		return m_PhysicalDevice;
	}

	VkDevice& VulkanDevice::GetLogicalDevice()
	{
		return m_Device;
	}

	VkCommandBuffer VulkanDevice::CreateCommandBuffer(VkCommandBufferLevel commandBufferLevel)
	{
		Application& app = Application::Get();
		VkCommandPool commandPool = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetSwapChain().GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = commandBufferLevel;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
	{
		Application& app = Application::Get();
		VkCommandPool commandPool = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetSwapChain().GetCommandPool();
		VulkanDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice();

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;

		VkFence fence;

		if (vkCreateFence(logicalDevice.GetLogicalDevice(), &fenceCreateInfo, nullptr, &fence) != VK_SUCCESS) {
			printf("Single time fence not created\n");
		}

		vkQueueSubmit(logicalDevice.GetGraphicsQueue(), 1, &submitInfo, fence);

		vkWaitForFences(logicalDevice.GetLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

		vkDestroyFence(logicalDevice.GetLogicalDevice(), fence, nullptr);

		vkFreeCommandBuffers(logicalDevice.GetLogicalDevice(), commandPool, 1, &commandBuffer);
	}

	void VulkanDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(m_Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			printf("Failed to create buffer\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			printf("Failed to allocate buffer memory\n");
		}

		vkBindBufferMemory(m_Device, buffer, bufferMemory, 0);
	}

	uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice.GetPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		return -1;
	}
}
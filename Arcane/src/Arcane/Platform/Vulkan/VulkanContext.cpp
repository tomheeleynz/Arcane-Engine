#include "VulkanContext.h"

#include "Arcane/Core/Application.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Arcane {

	VulkanContext::VulkanContext()
	{
		// -- Application Info
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Triangle Application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		// -- Create Instance Info
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// -- GLFW extensions
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
			printf("Instance Not Created\n");
		}
		else {
			printf("Vulkan Instance Created\n");
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Create Window Surface
		if (glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface) != VK_SUCCESS) {
			printf("Window Surface Not Created\n");
		}
		else {
			printf("Window Surface Created\n");
		}

		m_PhysicalDevice = new VulkanPhysicalDevice(m_Surface);
		m_Device = new VulkanDevice(*m_PhysicalDevice);
		m_SwapChain = new VulkanSwapChain(*m_Device, m_Surface);
	}

	void VulkanContext::SwapBuffers()
	{
		m_SwapChain->SwapBuffers();
	}
}
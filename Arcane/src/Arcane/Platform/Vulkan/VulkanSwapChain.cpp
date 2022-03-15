#include <GLFW/glfw3.h>

#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"

namespace Arcane {

    VulkanSwapChain::VulkanSwapChain(VulkanDevice& _device, VkSurfaceKHR _surface)
    {
        m_VulkanDevice = &_device;
        m_Surface = _surface;

        SwapChainSupportDetails& details = _device.GetVulkanPhysicalDevice().GetSwapChainDetails();
        QueueFamilyIndices& indices = _device.GetVulkanPhysicalDevice().GetIndices();

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(details.presentModes);
        VkExtent2D extent = ChooseSwapExtent(details.capabilities);

        m_Extent = extent;
        m_Format = surfaceFormat.format;
        m_PresentMode = presentMode;

        // Creation Functions
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
        CreateCommandPool();
        CreateCommandBuffers();

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        
        if (vkCreateSemaphore(m_VulkanDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_PresentCompleteSemaphore) != VK_SUCCESS) {
            printf("Present Complete Semaphore Not Created\n");
        }

        if (vkCreateSemaphore(m_VulkanDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_RenderCompleteSemaphore) != VK_SUCCESS) {
            printf("Render Complete Semaphore Not Created\n");
        }


        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        m_WaitFences.resize(m_CommandBuffers.size());

        for (VkFence& fence : m_WaitFences) {
            if (vkCreateFence(m_VulkanDevice->GetLogicalDevice(), &fenceInfo, nullptr, &fence)) {
                printf("Fence Not Created\n");
            }
        }
    }

    void VulkanSwapChain::CreateSwapChain() 
    {
        SwapChainSupportDetails& details = m_VulkanDevice->GetVulkanPhysicalDevice().QuerySupportDetails(m_Surface);
        QueueFamilyIndices& indices = m_VulkanDevice->GetVulkanPhysicalDevice().GetIndices();

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(details.presentModes);
        VkExtent2D extent = ChooseSwapExtent(details.capabilities);

        m_Extent = extent;
        m_Format = surfaceFormat.format;
        m_PresentMode = presentMode;

        // Get Image Count
        m_ImageCount = details.capabilities.minImageCount + 1;
        if (details.capabilities.maxImageCount > 0 && m_ImageCount > details.capabilities.maxImageCount) {
            m_ImageCount = details.capabilities.maxImageCount;
        }

        // Create Swap Chain
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_Surface;
        createInfo.minImageCount = m_ImageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_VulkanDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
            printf("Swapchain Not Created");
        } else {
            printf("Swapchain Created\n");
        }
    }

    void VulkanSwapChain::CreateImageViews() {
        vkGetSwapchainImagesKHR(m_VulkanDevice->GetLogicalDevice(), m_SwapChain, &m_ImageCount, nullptr);
        m_SwapChainImages.resize(m_ImageCount);
        vkGetSwapchainImagesKHR(m_VulkanDevice->GetLogicalDevice(), m_SwapChain, &m_ImageCount, m_SwapChainImages.data());

        // Create Swap Chain Images
        m_SwapChainImageViews.resize(m_SwapChainImages.size());

        for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_SwapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_Format;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_VulkanDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
                printf("Image View Not Created\n");
            }
            else {
                printf("Image View Created\n");
            }
        }
    }


    void VulkanSwapChain::CreateRenderPass() 
    {
        // Create Renderpass
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_Format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // Subpass Dependency
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;

        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;

        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_VulkanDevice->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
            printf("Swapchain Renderpass Not Created\n");
        }

    }

    void VulkanSwapChain::CreateFramebuffers() {
        m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

        // Create Swapchain Framebuffers
        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_Extent.width;
            framebufferInfo.height = m_Extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_VulkanDevice->GetLogicalDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
                printf("Framebuffers Not Created\n");
            }
            else {
                printf("Framebuffers Created\n");
            }
        }
    }

    void VulkanSwapChain::CreateCommandPool() 
    {
        QueueFamilyIndices& indices = m_VulkanDevice->GetVulkanPhysicalDevice().GetIndices();

        // Create Command Pool
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(m_VulkanDevice->GetLogicalDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
            printf("Command Pool Not Created\n");
        }
        else {
            printf("Command Pool Created\n");
        }
    }

    void VulkanSwapChain::CreateCommandBuffers() 
    {
        // Create Command Buffers
        m_CommandBuffers.resize(m_SwapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

        if (vkAllocateCommandBuffers(m_VulkanDevice->GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) {
            printf("Failed to allocate command buffers!\n");
        }
        else {
            printf("Allocated Command Buffers!\n");
        }
    }

    VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        else {
            Application& app = Application::Get();
            GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanSwapChain::RecreateSwapchain()
    {
        vkDeviceWaitIdle(m_VulkanDevice->GetLogicalDevice());

        // Destroy Framebuffers
        for (size_t i = 0; i < m_SwapChainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(m_VulkanDevice->GetLogicalDevice(), m_SwapChainFramebuffers[i], nullptr);
        }

        // Free Command Buffers
        vkFreeCommandBuffers(m_VulkanDevice->GetLogicalDevice(), m_CommandPool, static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
       
        // Destroy Image Views
        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
            vkDestroyImageView(m_VulkanDevice->GetLogicalDevice(), m_SwapChainImageViews[i], nullptr);
        }

        // Destroy actual swapchain
        vkDestroySwapchainKHR(m_VulkanDevice->GetLogicalDevice(), m_SwapChain, nullptr);

        // Recreate Things for swapchain
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
        CreateCommandPool();
        CreateCommandBuffers();

    }

    VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex)
    {
        Application& app = Application::Get();
        VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
        return vkAcquireNextImageKHR(_context->GetDevice().GetLogicalDevice(), m_SwapChain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, imageIndex);
    }

    VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
    {
        Application& app = Application::Get();
        VulkanContext* _context = static_cast<VulkanContext*>(app.GetWindow().GetContext());

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = NULL;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_SwapChain;
        presentInfo.pImageIndices = &imageIndex;

        if (waitSemaphore != VK_NULL_HANDLE)
        {
            presentInfo.pWaitSemaphores = &waitSemaphore;
            presentInfo.waitSemaphoreCount = 1;
        }

        return vkQueuePresentKHR(_context->GetDevice().GetPresentQueue(), &presentInfo);
    }

    void VulkanSwapChain::SwapBuffers()
    {
        VkResult result = AcquireNextImage(m_PresentCompleteSemaphore, &m_CurrentBuffer);

        vkWaitForFences(m_VulkanDevice->GetLogicalDevice(), 1, &m_WaitFences[m_CurrentBuffer], VK_TRUE, UINT64_MAX);
        vkResetFences(m_VulkanDevice->GetLogicalDevice(), 1, &m_WaitFences[m_CurrentBuffer]);


        VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = &waitStageMask;               
        submitInfo.pWaitSemaphores = &m_PresentCompleteSemaphore;     
        submitInfo.waitSemaphoreCount = 1;                          
        submitInfo.pSignalSemaphores = &m_RenderCompleteSemaphore;     
        submitInfo.signalSemaphoreCount = 1;                       
        submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentBuffer]; 
        submitInfo.commandBufferCount = 1;                          

        vkQueueSubmit(m_VulkanDevice->GetPresentQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBuffer]);

        VkResult present = QueuePresent(m_VulkanDevice->GetPresentQueue(), m_CurrentBuffer, m_RenderCompleteSemaphore);
        
        vkQueueWaitIdle(m_VulkanDevice->GetPresentQueue());


        if (present != VK_SUCCESS || present == VK_SUBOPTIMAL_KHR) {
            if (present == VK_ERROR_OUT_OF_DATE_KHR) {
                RecreateSwapchain();
            }
        }

        m_CurrentBuffer = (m_CurrentBuffer + 1) % m_MaxFramesInFlight;
    }
}
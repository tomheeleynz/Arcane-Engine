#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanImGuiLayer.h"
#include "VulkanSwapChain.h"

namespace Arcane {
	VulkanImGuiLayer::VulkanImGuiLayer()
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 10000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 10000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 10000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 10000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 10000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 10000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 10000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 10000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		VkDescriptorPool imguiPool;

		if (vkCreateDescriptorPool(context->GetDevice().GetLogicalDevice(), &pool_info, nullptr, &imguiPool) != VK_SUCCESS)
		{
			printf("ImGui Descriptor Pool Not Created\n");
		}
		else {
			printf("ImGui Descriptor Pool Created\n");
		}

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking for ImGui
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

		float fontSize = 18.0f;
		
		std::string boldFileLocation = std::string(Application::Get().GetEditorAssetPath()) + "\\src\\EditorAssets\\Fonts\\opensans\\OpenSans-Bold.ttf";
		std::string regularFileLocation = std::string(Application::Get().GetEditorAssetPath()) + "\\src\\EditorAssets\\Fonts\\opensans\\OpenSans-Regular.ttf";

		io.Fonts->AddFontFromFileTTF(boldFileLocation.c_str(), fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF(regularFileLocation.c_str(), fontSize);

		Application& app = Application::Get();
		GLFWwindow* windowHandle = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		VulkanContext* vulkanContext = static_cast<VulkanContext*>(app.GetWindow().GetContext());

		ImGui_ImplGlfw_InitForVulkan(windowHandle,true);

		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.Instance = vulkanContext->GetInstance();
		initInfo.PhysicalDevice = vulkanContext->GetPhysicalDevice().GetPhysicalDevice();
		initInfo.Device = vulkanContext->GetDevice().GetLogicalDevice();
		initInfo.Queue = vulkanContext->GetDevice().GetGraphicsQueue();
		initInfo.DescriptorPool = imguiPool;
		initInfo.MinImageCount = 3;
		initInfo.ImageCount = 3;
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&initInfo, vulkanContext->GetSwapChain().GetSwapchainRenderPass());
		
		VkCommandBuffer fontCommand = vulkanContext->GetDevice().CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		ImGui_ImplVulkan_CreateFontsTexture(fontCommand);

		vulkanContext->GetDevice().FlushCommandBuffer(fontCommand);
	}

	void VulkanImGuiLayer::OnAttach()
	{

	}

	void VulkanImGuiLayer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void VulkanImGuiLayer::End()
	{
		ImGui::Render();

		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanDevice device = context->GetDevice();

		auto swapChainCommandBuffers = context->GetSwapChain().GetCommandBuffers();
		auto swapChainFramebuffers = context->GetSwapChain().GetSwapChainFramebuffers();

		for (size_t i = 0; i < swapChainCommandBuffers.size(); i++)
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = context->GetSwapChain().GetSwapchainRenderPass();
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = context->GetSwapChain().GetExtent();

			VkClearValue clearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(swapChainCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), swapChainCommandBuffers[i]);
		}

		for (size_t i = 0; i < swapChainCommandBuffers.size(); i++)
		{
			vkCmdEndRenderPass(swapChainCommandBuffers[i]);
		}
	}
}

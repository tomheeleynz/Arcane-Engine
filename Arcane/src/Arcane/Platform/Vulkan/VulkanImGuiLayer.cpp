#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanImGuiLayer.h"

namespace Arcane {
	VulkanImGuiLayer::VulkanImGuiLayer()
	{
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
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
	}

	void VulkanImGuiLayer::OnAttach()
	{

	}

	void VulkanImGuiLayer::Begin()
	{

	}

	void VulkanImGuiLayer::End()
	{

	}
}

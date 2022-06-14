#include "Arcane/Core/Application.h"
#include "VulkanContext.h"
#include "VulkanSet.h"

namespace Arcane
{
	VulkanSet::VulkanSet(DescriptorSetSpecs& specs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs)
	{
		Application& app = Application::Get();
		VulkanContext* context = static_cast<VulkanContext*>(Application::Get().GetWindow().GetContext());
		VulkanSwapChain& swapchain = context->GetSwapChain();
		VkDevice logicalDevice = static_cast<VulkanContext*>(app.GetWindow().GetContext())->GetDevice().GetLogicalDevice();

		// Iterate through Layouts, create bindings for each
		// Manage the counts of each set
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		for (DescriptorLayoutSpecs spec : layoutSpecs) {
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = spec.Binding;
			binding.descriptorCount = spec.DescriptorCount;
			
			// Switch on the type
			switch (spec.Type)
			{
			case DescriptorType::UNIFORM_BUFFER: 
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case DescriptorType::SAMPLER:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			default:
				break;
			}

			bindings.push_back(binding);
		}

		// Actually Create Descriptor set
		VkDescriptorSetLayoutCreateInfo setInfo = {};
		setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setInfo.pNext = nullptr;
		setInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		setInfo.pBindings = bindings.data();
		
		if (vkCreateDescriptorSetLayout(logicalDevice, &setInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
			printf("Uniform Descriptor Layout Not Created\n");
		}

		// Get Descriptor Pool from context
		VkDescriptorPool descriptorPool = context->GetPool();
		uint32_t imageCount = swapchain.GetSwapChainImagesSize();
		m_DescriptorSets.resize(imageCount);
		for (int i = 0; i < imageCount; i++) {
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_DescriptorSetLayout;

			if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &m_DescriptorSets[i]) != VK_SUCCESS) {
				printf("Failed to allocate descripor set\n");
			}
		}

	}
}
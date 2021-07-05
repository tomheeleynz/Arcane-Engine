#include "VulkanUniformBuffer.h"
#include "Core/Application.h"
#include "VulkanContext.h"

VulkanUniformBuffer::VulkanUniformBuffer()
{
	Application& app = Application::Get();
	VulkanContext* context = static_cast<VulkanContext*>(app.GetWindow().GetContext());
	VkDevice logicalDevice = context->GetDevice().GetLogicalDevice();

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
		printf("Descriptor Set Layout Not Created\n");
	}
	else {
		printf("Descriptor Set Created\n");
	}
}
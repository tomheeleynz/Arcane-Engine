#pragma once

#include <vulkan/vulkan.h>

namespace Arcane
{
	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet();

		VkDescriptorSetLayout GetLayout() { return m_Layout; }
	private:
		VkDescriptorSetLayout m_Layout;
	};
}
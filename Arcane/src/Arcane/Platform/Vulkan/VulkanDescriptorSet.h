#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Arcane
{
	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet(std::vector<VkBuffer> uniformBuffers, uint32_t size);

		VkDescriptorSetLayout GetLayout() { return m_Layout; }
		std::vector<VkDescriptorSet> GetDescriptorSets() { return m_DescriptorSets; }
	private:
		VkDescriptorSetLayout m_Layout;
		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorSet> m_DescriptorSets;
	};
}
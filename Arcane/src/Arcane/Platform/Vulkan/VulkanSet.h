#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Arcane/Renderer/DescriptorSet.h"

namespace Arcane
{
	class VulkanSet : public DescriptorSet
	{
	public:
		VulkanSet(DescriptorSetSpecs& specs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs);

	private:
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> m_DescriptorSets;
	};
}
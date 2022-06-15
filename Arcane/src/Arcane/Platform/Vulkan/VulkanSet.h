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

		// Virtual Functions
		void AddUniformBuffer(UniformBuffer* buffer, uint32_t setNumber, uint32_t bindingNumber) override;
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> m_DescriptorSets;
	};
}
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
		void AddImageSampler(Texture* texture, uint32_t setNumber, uint32_t bindingNumber) override;
		void AddImageSampler(Framebuffer* framebuffer, uint32_t setNumber, uint32_t bindingNumber) override;


		// Get Descriptor Set Layout
		VkDescriptorSetLayout GetLayout() { return m_DescriptorSetLayout; }
		std::vector<VkDescriptorSet> GetDescriptorSets() { return m_DescriptorSets; }

		// Get Set Number
		uint32_t GetSetNumber() { return m_SetNumber; }
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> m_DescriptorSets;

		// Set Number
		uint32_t m_SetNumber;
	};
}
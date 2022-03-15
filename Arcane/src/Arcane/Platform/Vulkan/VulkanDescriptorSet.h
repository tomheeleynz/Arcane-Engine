#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "Arcane/Renderer/UniformBuffer.h"
#include "VulkanTexture.h"

namespace Arcane
{
	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet(std::vector<VkBuffer> uniformBuffers, std::initializer_list<UniformDescriptor*> descriptors);
		VulkanDescriptorSet(std::initializer_list<UniformDescriptor*> descriptors);

		VkDescriptorSetLayout GetLayout() { return m_Layout; }
		std::vector<VkDescriptorSet> GetDescriptorSets() { return m_DescriptorSets; }
	private:
		VkDescriptorSetLayout m_Layout;
		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorSet> m_DescriptorSets;
	};
}
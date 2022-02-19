#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Arcane/Renderer/UniformBuffer.h"
#include "VulkanDescriptorSet.h"

namespace Arcane
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(std::initializer_list<UniformDescriptor*> descriptors);

		void WriteData(void* data, uint32_t size) override;
		void WriteData(UniformObject* object) override;

		VkDescriptorSetLayout GetLayout() { return m_DescriptorSet->GetLayout(); }
		std::vector<VkDescriptorSet> GetDescriptorSets() { return m_DescriptorSet->GetDescriptorSets(); }
	private:
		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBuffersMemory;
		VulkanDescriptorSet* m_DescriptorSet;
	};
}
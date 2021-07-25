#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <glm/glm.hpp>

#include "Arcane/Renderer/UniformBuffer.h"

namespace Arcane {
	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(uint32_t size);

		// Get Descriptor Layout to give to pipeline
		VkDescriptorSetLayout GetLayout() { return m_DescriptorLayout; }

		// Write Data
		void WriteData(void* data) override;
	private:
		// Descriptor Set Layout
		VkDescriptorSetLayout m_DescriptorLayout;

		// Descriptor Pool
		VkDescriptorPool m_DescriptorPool;

		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBuffersMemory;
		std::vector<VkDescriptorSet> m_DescriptorSets;

		// Uniform Buffer Size
		uint32_t m_Size;
	};
}
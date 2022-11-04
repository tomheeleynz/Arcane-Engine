#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Arcane/Renderer/UniformBuffer.h"

namespace Arcane
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(uint32_t size);

		void WriteData(void* data, uint32_t size) override;
		std::vector<VkBuffer> GetUniformBuffers() { return m_UniformBuffers; }

		uint32_t GetSize() override;
	private:
		uint32_t m_Size = 0; 
		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	};
}
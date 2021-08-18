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

		void WriteData(void* data) override;
	private:
		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	};
}
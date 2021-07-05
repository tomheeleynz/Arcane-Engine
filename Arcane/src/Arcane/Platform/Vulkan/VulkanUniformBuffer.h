#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/UniformBuffer.h"

class VulkanUniformBuffer : public UniformBuffer
{
public:
	VulkanUniformBuffer();


private:
	VkDescriptorSetLayout m_DescriptorSetLayout;
};
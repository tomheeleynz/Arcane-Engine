#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <glm/glm.hpp>

#include "Renderer/UniformBuffer.h"

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class VulkanUniformBuffer : public UniformBuffer
{
public:
	VulkanUniformBuffer();


private:
	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> m_UniformBuffersMemory;
};
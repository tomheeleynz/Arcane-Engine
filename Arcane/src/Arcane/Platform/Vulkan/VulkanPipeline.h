#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Pipeline.h"

class VulkanPipeline : public Pipeline
{
public:
	VulkanPipeline(PipelineSpecification& spec);

	VkPipeline GetPipeline() { return m_Pipeline; }
private:
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_Pipeline;
	VkDescriptorSetLayout m_DescriptorLayout;
};
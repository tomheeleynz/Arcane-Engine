#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/Pipeline.h"

namespace Arcane {
	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(PipelineSpecification& spec);

		VkPipelineLayout GetLayout() { return m_PipelineLayout; }
		VkPipeline GetPipeline() { return m_Pipeline; }
	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;
		VkDescriptorSetLayout m_DescriptorLayout;
	};
}
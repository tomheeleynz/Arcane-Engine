#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/RenderPass.h"

namespace Arcane {
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(RenderPassSpecs specs);

		VkRenderPass GetRenderPass() { return m_RenderPass; }

		// Inherited via RenderPass
		virtual RenderPassSpecs& GetRenderPassSpecs() override;
	private:
		VkRenderPass m_RenderPass;
		RenderPassSpecs m_Specifications;

	};
}
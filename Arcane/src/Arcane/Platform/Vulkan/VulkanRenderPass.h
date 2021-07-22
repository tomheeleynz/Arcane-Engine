#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/RenderPass.h"

namespace Arcane {
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass();

		VkRenderPass GetRenderPass() { return m_RenderPass; }
	private:
		VkRenderPass m_RenderPass;

	};
}
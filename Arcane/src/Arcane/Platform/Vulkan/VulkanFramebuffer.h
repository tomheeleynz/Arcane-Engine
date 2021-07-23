#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/Framebuffer.h"

namespace Arcane {
	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(FramebufferSpecifications& specs);

	private:

	};
}
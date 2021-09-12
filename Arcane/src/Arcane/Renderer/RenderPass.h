#pragma once

#include <glm/glm.hpp>
#include "Framebuffer.h"

namespace Arcane {
	struct RenderPassSpecs
	{
		Framebuffer* TargetFramebuffer;
	};
	
	class RenderPass
	{
	public:
		virtual RenderPassSpecs& GetRenderPassSpecs() = 0;
		static RenderPass* Create(RenderPassSpecs specs);
	private:

	};
}
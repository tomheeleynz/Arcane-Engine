#pragma once

#include "Arcane/Renderer/RenderPass.h"

namespace Arcane
{
	class OpenGLRenderPass : public RenderPass
	{
	public:
		OpenGLRenderPass(RenderPassSpecs specs);

		RenderPassSpecs& GetRenderPassSpecs() override;
	private:
		RenderPassSpecs m_Specs;
	
	};
}
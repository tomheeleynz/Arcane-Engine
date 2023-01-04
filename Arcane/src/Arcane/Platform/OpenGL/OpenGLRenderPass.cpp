#include "OpenGLRenderPass.h"

namespace Arcane 
{
	OpenGLRenderPass::OpenGLRenderPass(RenderPassSpecs specs)
	{
		m_Specs = specs;
	}
	
	RenderPassSpecs& OpenGLRenderPass::GetRenderPassSpecs()
	{
		return m_Specs;
	}
}
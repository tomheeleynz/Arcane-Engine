#pragma once

#include "Arcane/Renderer/Framebuffer.h"

namespace Arcane
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecifications& specs);

		FramebufferSpecifications GetSpecs() override;
		void Resize(uint32_t width, uint32_t height) override;

		void Bind();
		void UnBind();
	private:
		uint32_t m_FBO;
		uint32_t m_FramebufferTexture;
		FramebufferSpecifications m_Specs;
	};
}
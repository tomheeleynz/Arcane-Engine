#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Arcane
{
	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecifications& specs)
	{
		m_Specs = specs;

		// Generate Framebuffer
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Generate Texture
		glGenTextures(1, &m_FramebufferTexture);
		glBindTexture(GL_TEXTURE_2D, m_FramebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specs.Width, specs.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// Set Texture Params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer Complete\n";

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FramebufferSpecifications OpenGLFramebuffer::GetSpecs()
	{
		return m_Specs;
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{

	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
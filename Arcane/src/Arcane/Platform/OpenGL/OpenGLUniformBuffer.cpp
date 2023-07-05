#include "OpenGLUniformBuffer.h"

namespace Arcane
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size)
	{
		m_Size = size;
		glGenBuffers(1, &m_UboID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UboID);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW); 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	uint32_t OpenGLUniformBuffer::GetSize()
	{
		return m_Size;
	}

	void OpenGLUniformBuffer::WriteData(void* data, uint32_t size)
	{
		glBufferSubData(m_UboID, 0, size, data);
	}
}
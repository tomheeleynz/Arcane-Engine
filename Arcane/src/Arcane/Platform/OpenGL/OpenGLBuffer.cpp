#include "OpenGLBuffer.h"

#include <iostream>
#include <glad/glad.h>

namespace Arcane
{
	////////////////////////////////////////////////////////
	//// Vertex Buffer
	////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size)
	{
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::AddIndexBuffer(IndexBuffer* _indexBuffer)
	{
		m_IndexBuffer = _indexBuffer;
	}

	IndexBuffer* OpenGLVertexBuffer::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t size)
	{
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void OpenGLVertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	////////////////////////////////////////////////////////
	//// Index Buffer
	////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count)
	{
		m_Count = count;
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount()
	{
		return m_Count;
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	}

	void OpenGLIndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
#pragma once
#include "Arcane/Renderer/Buffer.h"

#include <vector>

namespace Arcane
{
	////////////////////////////////////////////////////////
	//// Vertex Buffer
	////////////////////////////////////////////////////////
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, uint32_t size);

		void AddIndexBuffer(IndexBuffer* _indexBuffer) override;
		IndexBuffer* GetIndexBuffer() override;

		void Bind();
		void UnBind();
	private:
		IndexBuffer* m_IndexBuffer;
		uint32_t m_VBO;
	};

	////////////////////////////////////////////////////////
	//// Index Buffer
	////////////////////////////////////////////////////////
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data, uint32_t count);
		uint32_t GetCount();

		void Bind();
		void UnBind();
	private:
		uint32_t m_EBO;
		uint32_t m_Count;
	};

}
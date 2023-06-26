#include "Quad.h"

#include <vector>

namespace Arcane
{
	Quad::Quad()
	{
		std::vector<QuadVertex> vertices = {
			{{0.5f,  0.5f, 0.0f},   {1.0f, 0.0f, 0.0f},   {1.0f, 1.0f}},
			{{0.5f, -0.5f, 0.0f},   {0.0f, 1.0f, 0.0f},   {1.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f},   {0.0f, 0.0f}},
			{{-0.5f,  0.5f, 0.0f},  {1.0f, 1.0f, 0.0f},   {0.0f, 1.0f}}
		};

		std::vector<uint32_t> indices = {
			0, 1, 3,
			1, 2, 3  
		};

		// Create vertex buffer
		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(QuadVertex));
		
		// Create Index Buffer
		m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());

		// Add Index Buffer to Vertex Buffer
		m_VertexBuffer->AddIndexBuffer(m_IndexBuffer);
	}
}
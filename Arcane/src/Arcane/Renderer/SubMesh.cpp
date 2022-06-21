#include "SubMesh.h"

namespace Arcane
{
	SubMesh::SubMesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices)
	{
		// Create Vertex Buffer
		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(MeshVertex));

		// Create Index Buffer
		m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());

		// Add Index Buffer to Vertex Buffer
		m_VertexBuffer->AddIndexBuffer(m_IndexBuffer);
	}
}
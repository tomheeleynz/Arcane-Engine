#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Buffer.h"

namespace Arcane
{
	struct MeshVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;
	};

	class SubMesh
	{
	public:
		SubMesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices);

		VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
		IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }
	private:
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
	};
}
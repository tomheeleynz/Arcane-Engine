#pragma once

#include <glm/glm.hpp>

#include "Buffer.h"

namespace Arcane
{
	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
	};

	class Quad
	{
	public:
		Quad();

		VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
	private:
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;

	};
}
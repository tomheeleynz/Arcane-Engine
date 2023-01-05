#pragma once

#include "Arcane/Renderer/VertexDescriptor.h"
#include "Arcane/Renderer/Buffer.h"

#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace Arcane
{
	class OpenGLVertexDescriptor : public VertexDescriptor
	{
	public:
		OpenGLVertexDescriptor(std::initializer_list<VertexType> vertexTypes);

		void Bind();
		void UnBind();

		bool GetIsGenerated() { return m_IsGenerated; }
		void SetIsGenerated(bool isGenerated) { m_IsGenerated = isGenerated; }

		void Generate(VertexBuffer* buffer);
	private:
		uint32_t m_VAO;
		bool m_IsGenerated = false;

		struct VertexAttribSpec
		{
			uint32_t offset;
			float stride;
			VertexType type;
			float size;
			int count;
		};

		std::vector<VertexAttribSpec> m_AttribSpecs;
	};
}
#pragma once

#include "Arcane/Renderer/VertexDescriptor.h"

#include <glad/glad.h>
#include <iostream>

namespace Arcane
{
	class OpenGLVertexDescriptor : public VertexDescriptor
	{
	public:
		OpenGLVertexDescriptor(std::initializer_list<VertexType> vertexTypes);

		void Bind();
		void UnBind();
	private:
		uint32_t m_VAO;
	
	};
}
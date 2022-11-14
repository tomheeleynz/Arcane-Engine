#include "OpenGLVertexDescriptor.h"

namespace Arcane
{
	OpenGLVertexDescriptor::OpenGLVertexDescriptor(std::initializer_list<VertexType> vertexTypes)
	{
		// Generate VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Calculate Stride
		uint32_t stride = 0;
		for (auto element : vertexTypes)
		{
			switch (element)
			{
			case VertexType::float1:
			{
				stride += sizeof(float);
				break;
			}
			case VertexType::float2:
			{
				stride += sizeof(float) * 2;
				break;
			}
			case VertexType::float3:
			{
				stride += sizeof(float) * 3;
				break;
			}
			case VertexType::float4:
			{
				stride += sizeof(float) * 4;
				break;
			}
			default:
				break;
			}
		}
		
		// Attribute Arrays
		uint32_t count = 0;
		uint32_t offset = 0;
		for (auto element : vertexTypes)
		{
			switch (element)
			{
			case VertexType::float1:
			{
				stride += sizeof(float);
				break;
			}
			case VertexType::float2:
			{
				stride += sizeof(float) * 2;
				break;
			}
			case VertexType::float3:
			{
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
				glEnableVertexAttribArray(count);
				offset += 3 * sizeof(float);
				count += 1;
				break;
			}
			case VertexType::float4:
			{
				stride += sizeof(float) * 4;
				break;
			}
			default:
				break;
			}
		}

		glBindVertexArray(0);
	}

	void OpenGLVertexDescriptor::Bind()
	{
		glBindVertexArray(m_VAO);
	}

	void OpenGLVertexDescriptor::UnBind()
	{
		glBindVertexArray(0);
	}
}
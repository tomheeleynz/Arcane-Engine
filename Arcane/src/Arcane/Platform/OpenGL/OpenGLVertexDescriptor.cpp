#include "OpenGLVertexDescriptor.h"
#include "OpenGLBuffer.h"

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
				break;
			}
			case VertexType::float2:
			{
				VertexAttribSpec newSpec;
				newSpec.offset = offset;
				newSpec.stride = stride;
				newSpec.type = VertexType::float2;
				newSpec.size = 2;
				newSpec.count = count;
				m_AttribSpecs.push_back(newSpec);

				offset += sizeof(float) * 2;
				count += 1;
				break;
			}
			case VertexType::float3:
			{
				VertexAttribSpec newSpec;
				newSpec.offset = offset;
				newSpec.stride = stride;
				newSpec.type = VertexType::float3;
				newSpec.size = 3;
				newSpec.count = count;
				m_AttribSpecs.push_back(newSpec);

				offset += sizeof(float) * 3;
				count += 1;
				break;
			}
			case VertexType::float4:
			{
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

	void OpenGLVertexDescriptor::Generate(VertexBuffer* buffer)
	{
		// Bind Vertex Array
		glBindVertexArray(m_VAO);
		
		// Bind Vertex Buffer
		OpenGLVertexBuffer* openglBuffer = static_cast<OpenGLVertexBuffer*>(buffer);
		OpenGLIndexBuffer* indexBuffer = static_cast<OpenGLIndexBuffer*>(buffer->GetIndexBuffer());
		
		openglBuffer->Bind();
		indexBuffer->Bind();

		// Set Attrib Array
		for (int i = 0; i < m_AttribSpecs.size(); i++) {
			VertexAttribSpec spec = m_AttribSpecs[i];
			glVertexAttribPointer(spec.count, spec.size, GL_FLOAT, GL_FALSE, spec.stride, (const void*)spec.offset);
			glEnableVertexAttribArray(spec.count);
		}

		// Cleanup
		openglBuffer->UnBind();
		indexBuffer->UnBind();
		glBindVertexArray(0);
		m_IsGenerated = true;
	}
}
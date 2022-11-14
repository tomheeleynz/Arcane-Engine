#include "OpenGLPipeline.h"

#include "OpenGLShader.h"
#include "OpenGLVertexDescriptor.h"

namespace Arcane
{
	OpenGLPipeline::OpenGLPipeline(PipelineSpecification& spec)
	{
		m_Spec = spec;
	}

	void OpenGLPipeline::BindShader()
	{
		OpenGLShader* shader = static_cast<OpenGLShader*>(m_Spec.shader);
		shader->Bind();
	}

	void OpenGLPipeline::BindVertexDescriptor()
	{
		OpenGLVertexDescriptor* vertexDescriptor = static_cast<OpenGLVertexDescriptor*>(m_Spec.descriptor);
		vertexDescriptor->Bind();
	}

	void OpenGLPipeline::UnbindShader()
	{
		OpenGLShader* shader = static_cast<OpenGLShader*>(m_Spec.shader);
		shader->UnBind();
	}

	void OpenGLPipeline::UnbindVertexDescriptor()
	{
		OpenGLVertexDescriptor* vertexDescriptor = static_cast<OpenGLVertexDescriptor*>(m_Spec.descriptor);
		vertexDescriptor->UnBind();
	}
}
#pragma once
#include "Arcane/Renderer/Pipeline.h"

namespace Arcane
{
	class OpenGLPipeline : public Pipeline 
	{
	public:
		OpenGLPipeline(PipelineSpecification& spec);

		void BindShader();
		void BindVertexDescriptor();

		void UnbindShader();
		void UnbindVertexDescriptor();

		PipelineSpecification GetSpec();
	private:
		PipelineSpecification m_Spec;
	};
}
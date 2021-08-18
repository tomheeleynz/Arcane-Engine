#pragma once

#include "Shader.h"
#include "RenderPass.h"
#include "VertexDescriptor.h"
#include "UniformBuffer.h"

namespace Arcane {
	struct PipelineSpecification
	{
		Shader* shader;
		RenderPass* renderPass;
		VertexDescriptor* descriptor;
		UniformBuffer* uniformBuffer;
	};

	class Pipeline
	{
	public:

		static Pipeline* Create(PipelineSpecification& spec);
	private:

	};
}
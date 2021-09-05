#pragma once

#include "Shader.h"
#include "RenderPass.h"
#include "VertexDescriptor.h"
#include "UniformBuffer.h"

namespace Arcane {
	struct PipelineSpecification
	{
		Shader* shader = nullptr;
		RenderPass* renderPass = nullptr;
		VertexDescriptor* descriptor = nullptr;
		UniformBuffer* uniformBuffer = nullptr;
	};

	class Pipeline
	{
	public:

		static Pipeline* Create(PipelineSpecification& spec);
	private:

	};
}
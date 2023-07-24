#pragma once
#include <vector>

#include "Shader.h"
#include "RenderPass.h"
#include "VertexDescriptor.h"
#include "DescriptorSet.h"

namespace Arcane {
	enum class PrimativeTopology
	{
		LINE_LIST,
		TRIANGLE_STRIP
	};

	struct PipelineSpecification
	{
		Shader* shader = nullptr;
		RenderPass* renderPass = nullptr;
		VertexDescriptor* descriptor = nullptr;
		std::vector<DescriptorSet*> DescriptorSets;
		PrimativeTopology topolgy;
	};

	class Pipeline
	{
	public:

		static Pipeline* Create(PipelineSpecification& spec);
	private:

	};
}
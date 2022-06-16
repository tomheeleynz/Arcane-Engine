#pragma once
#include <vector>

#include "Shader.h"
#include "RenderPass.h"
#include "VertexDescriptor.h"
#include "DescriptorSet.h"

namespace Arcane {
	struct PipelineSpecification
	{
		Shader* shader = nullptr;
		RenderPass* renderPass = nullptr;
		VertexDescriptor* descriptor = nullptr;
		std::vector<DescriptorSet*> DescriptorSets;
	};

	class Pipeline
	{
	public:

		static Pipeline* Create(PipelineSpecification& spec);
	private:

	};
}
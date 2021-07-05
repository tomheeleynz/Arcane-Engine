#pragma once

#include "Shader.h"
#include "RenderPass.h"
#include "VertexDescriptor.h"

struct PipelineSpecification
{
	Shader* shader;
	RenderPass* renderPass;
	VertexDescriptor* descriptor;
};

class Pipeline
{
public:

	static Pipeline* Create(PipelineSpecification& spec);
private:

};
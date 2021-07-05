#include "Pipeline.h"
#include "Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanPipeline.h"

Pipeline* Pipeline::Create(PipelineSpecification& spec)
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::Vulkan: return new VulkanPipeline(spec);
	default: return nullptr;
	}
}
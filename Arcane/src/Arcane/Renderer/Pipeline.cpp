#include "Pipeline.h"
#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Platform/Vulkan/VulkanPipeline.h"

namespace Arcane {
	Pipeline* Pipeline::Create(PipelineSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanPipeline(spec);
		default: return nullptr;
		}
	}
}
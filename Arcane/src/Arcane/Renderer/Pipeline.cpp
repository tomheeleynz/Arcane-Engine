#include "Pipeline.h"
#include "Arcane/Renderer/Renderer.h"

#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanPipeline.h"
#endif

namespace Arcane {
	Pipeline* Pipeline::Create(PipelineSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanPipeline(spec);
#endif
		default: return nullptr;
		}
	}
}

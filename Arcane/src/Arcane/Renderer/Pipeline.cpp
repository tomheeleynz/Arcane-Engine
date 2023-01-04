#include "Pipeline.h"
#include "Arcane/Renderer/Renderer.h"

// -- Vulkan Pipeline
#include "Arcane/Platform/Vulkan/VulkanPipeline.h"

// -- OpenGL Pipeline
#include "Arcane/Platform/OpenGL/OpenGLPipeline.h"

namespace Arcane {
	Pipeline* Pipeline::Create(PipelineSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanPipeline(spec);
		case RendererAPIType::OpenGL: return new OpenGLPipeline(spec);
		default: return nullptr;
		}
	}
}
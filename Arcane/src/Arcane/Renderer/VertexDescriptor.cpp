#include "VertexDescriptor.h"
#include "Renderer.h"

// Platform Specific
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanVertexDescriptor.h"

namespace Arcane {
	VertexDescriptor* VertexDescriptor::Create(std::initializer_list<VertexType> vertexTypes)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanVertexDescriptor(vertexTypes);
		default: return nullptr;
		}

	}
}
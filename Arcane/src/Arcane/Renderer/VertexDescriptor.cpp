#include "VertexDescriptor.h"
#include "Renderer.h"

// Platform Specific
// -- Vulkan
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanVertexDescriptor.h"
#endif

namespace Arcane {
	VertexDescriptor* VertexDescriptor::Create(std::initializer_list<VertexType> vertexTypes)
	{
		switch (RendererAPI::Current())
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanVertexDescriptor(vertexTypes);
#endif
		default: return nullptr;
		}

	}
}

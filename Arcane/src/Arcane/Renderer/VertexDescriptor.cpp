#include "VertexDescriptor.h"
#include "Renderer.h"

// Platform Specific
// -- Vulkan
#include "Platform/Vulkan/VulkanVertexDescriptor.h"

VertexDescriptor* VertexDescriptor::Create(std::initializer_list<VertexType> vertexTypes)
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::Vulkan: return new VulkanVertexDescriptor(vertexTypes);
	default: return nullptr;
	}
	
}
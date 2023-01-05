#include "VertexDescriptor.h"
#include "Renderer.h"

// Platform Specific
// -- Vulkan
#include "Arcane/Platform/Vulkan/VulkanVertexDescriptor.h"

// -- OpenGL
#include "Arcane/Platform/OpenGL/OpenGLVertexDescriptor.h"

namespace Arcane {
	VertexDescriptor* VertexDescriptor::Create(std::initializer_list<VertexType> vertexTypes)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanVertexDescriptor(vertexTypes);
		case RendererAPIType::OpenGL: return new OpenGLVertexDescriptor(vertexTypes);
		default: return nullptr;
		}

	}
}
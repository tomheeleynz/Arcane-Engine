#include "DescriptorSet.h"
#include "RendererAPI.h"

#if defined(ARCANE_WIN32)
	#include "Arcane/Platform/Vulkan/VulkanSet.h"
	#include "Arcane/Platform/OpenGL/OpenGLDescriptorSet.h"
#endif

namespace Arcane
{
	DescriptorSet* DescriptorSet::Create(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanSet(setSpecs, layoutSpecs);
		case RendererAPIType::OpenGL: return new OpenGLDescriptorSet(setSpecs, layoutSpecs);
		default: return nullptr;
		}
	}

	DescriptorSet* DescriptorSet::Create(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanSet(setSpecs, layoutSpecs);
		case RendererAPIType::OpenGL: return new OpenGLDescriptorSet(setSpecs, layoutSpecs);
		default: return nullptr;
		}
	}
}
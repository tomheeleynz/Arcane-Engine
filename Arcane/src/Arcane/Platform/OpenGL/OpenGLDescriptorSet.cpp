#include "OpenGLDescriptorSet.h"

namespace Arcane
{
	OpenGLDescriptorSet::OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs)
	{

	}

	OpenGLDescriptorSet::OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs)
	{

	}

	void OpenGLDescriptorSet::AddImageSampler(Texture* texture, uint32_t setNum, uint32_t bindingNum)
	{

	}

	void OpenGLDescriptorSet::AddImageSampler(Framebuffer* framebuffer, uint32_t setNum, uint32_t bindingNum)
	{

	}

	void OpenGLDescriptorSet::AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum)
	{

	}
}
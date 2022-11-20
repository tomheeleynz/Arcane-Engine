#pragma once

#include "Arcane/Renderer/DescriptorSet.h"

namespace Arcane
{
	class OpenGLDescriptorSet : public DescriptorSet
	{
	public:
		OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs);
		OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs);


		void AddImageSampler(Texture* texture, uint32_t setNum, uint32_t bindingNum) override;
		void AddImageSampler(Framebuffer* framebuffer, uint32_t setNum, uint32_t bindingNum) override;
		void AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum) override;
	private:
	
	};
}
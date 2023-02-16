#pragma once

#include <iostream>
#include <initializer_list>
#include <vector>

#include "UniformBuffer.h"
#include "Texture.h"
#include "Framebuffer.h"

namespace Arcane
{
	enum class DescriptorType
	{
		UNIFORM_BUFFER,
		SAMPLER
	};
	
	enum class DescriptorLocation
	{
		VERTEX,
		FRAGMENT
	};

	struct DescriptorSetSpecs
	{
		 uint32_t SetNumber;
	};

	struct DescriptorLayoutSpecs
	{
		uint32_t Binding;
		uint32_t DescriptorCount;
		DescriptorType Type;
		std::string Name;
		DescriptorLocation Location;
	};

	class DescriptorSet
	{
	public:
		virtual void AddImageSampler(Texture* texture, uint32_t setNum, uint32_t bindingNum) = 0;
		virtual void AddImageSampler(Framebuffer* framebuffer, uint32_t setNum, uint32_t bindingNum) = 0;
		virtual void AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum) = 0;
	
		static DescriptorSet* Create(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs);
		static DescriptorSet* Create(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs);
	private:

	};
}
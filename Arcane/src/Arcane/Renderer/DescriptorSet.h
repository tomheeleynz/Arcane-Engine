#pragma once

#include <iostream>
#include <initializer_list>

#include "UniformBuffer.h"

namespace Arcane
{
	enum class DescriptorType
	{
		UNIFORM_BUFFER,
		SAMPLER
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
	};

	class DescriptorSet
	{
	public:
		virtual void AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum) = 0;
		static DescriptorSet* Create(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs);
	private:

	};
}
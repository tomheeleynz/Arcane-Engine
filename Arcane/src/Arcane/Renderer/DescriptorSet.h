#pragma once

#include <iostream>
#include <initializer_list>

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
		static DescriptorSet* Create(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs);
	private:

	};
}
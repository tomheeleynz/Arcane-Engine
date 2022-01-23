#pragma once

#include <iostream>
#include "Texture.h"

namespace Arcane
{
	enum class UniformDescriptorType 
	{

	};

	class UniformBuffer
	{
	public:
		virtual void WriteData(void* data, uint32_t size) = 0;

		static UniformBuffer* Create(uint32_t size);
		static UniformBuffer* Create(Texture* texture, uint32_t size);
	private:

	};
}
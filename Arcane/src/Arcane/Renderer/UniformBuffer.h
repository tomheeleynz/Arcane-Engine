#pragma once

#include <iostream>

namespace Arcane
{
	class UniformBuffer
	{
	public:
		virtual void WriteData(void* data, uint32_t size) = 0;

		static UniformBuffer* Create(uint32_t size);
	private:

	};
}
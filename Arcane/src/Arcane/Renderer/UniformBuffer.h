#pragma once

#include <iostream>

namespace Arcane {
	class UniformBuffer
	{
	public:
		virtual void WriteData(void* data) = 0;
		static UniformBuffer* Create(uint32_t size);
	private:

	};
}
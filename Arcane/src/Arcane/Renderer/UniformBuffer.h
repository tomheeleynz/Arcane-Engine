#pragma once

#include <iostream>
#include <initializer_list>

#include "Framebuffer.h"
#include "Texture.h"

namespace Arcane
{
	class UniformBuffer
	{
	public:
		virtual uint32_t GetSize() = 0;
		virtual void WriteData(void* data, uint32_t size) = 0;
		static UniformBuffer* Create(uint32_t size);
	private:

	};
}
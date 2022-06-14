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
		virtual void WriteData(void* data, uint32_t size) = 0;
		static UniformBuffer* Create(uint32_t size);
	private:

	};
}
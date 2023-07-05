#pragma once

#include <glad/glad.h>
#include "Arcane/Renderer/UniformBuffer.h"

namespace Arcane
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size);

		uint32_t GetSize() override;
		void WriteData(void* data, uint32_t size) override;
	private:
		uint32_t m_Size;
		uint32_t m_UboID;
	};
}
#pragma once

#include <iostream>
#include <initializer_list>
#include "Texture.h"

namespace Arcane
{
	enum class UniformDescriptorType
	{
		UniformBufferObject,
		TextureSampler
	};

	class UniformDescriptor
	{
	public:
		UniformDescriptor(UniformDescriptorType _type) { m_Type = _type; }

		UniformDescriptorType GetType() { return m_Type; }
	private:
		UniformDescriptorType m_Type;
	};

	class UniformObject : public UniformDescriptor
	{
	public:
		UniformObject(uint32_t size)  : UniformDescriptor(UniformDescriptorType::UniformBufferObject) {
			m_Size = size;
		}

		uint32_t GetSize() { return m_Size; }

	private:
		uint32_t m_Size;

	};

	class TextureSampler : public UniformDescriptor
	{
	public:
		TextureSampler(Texture* texture) : UniformDescriptor(UniformDescriptorType::TextureSampler) {
			m_Texture = texture;
		}

	private:
		Texture* m_Texture = nullptr;

	};


	class UniformBuffer
	{
	public:
		virtual void WriteData(void* data, uint32_t size) = 0;

		static UniformBuffer* Create(std::initializer_list<UniformDescriptor> descriptors);
		static UniformBuffer* Create(uint32_t size);
		static UniformBuffer* Create(Texture* texture, uint32_t size);
	private:

	};
}
#pragma once

#include <iostream>
#include <initializer_list>

#include "Framebuffer.h"
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
		UniformDescriptor(UniformDescriptorType _type) { 
			m_Type = _type; 
		}

		void SetBinding(uint32_t bindNum) { m_BindNum = bindNum; }
		uint32_t GetBinding() { return m_BindNum; }

		UniformDescriptorType GetType() { return m_Type; }
	private:
		uint32_t m_BindNum = -1;
		UniformDescriptorType m_Type;
	};

	class UniformObject : public UniformDescriptor
	{
	public:
		UniformObject(uint32_t size)  : UniformDescriptor(UniformDescriptorType::UniformBufferObject) {
			m_Size = size;
		}

		uint32_t GetSize() { return m_Size; }
		void* GetData() { return m_Data; }

		void WriteData(void* data) { m_Data = data; };
	private:
		uint32_t m_Size;
		void* m_Data = nullptr;

	};

	class TextureSampler : public UniformDescriptor
	{
	public:
		enum class TextureType
		{
			BASIC,
			FRAMEBUFFER
		};
	public:
		TextureSampler(Texture* texture) : UniformDescriptor(UniformDescriptorType::TextureSampler) {
			m_Texture = texture;
			m_Type = TextureType::BASIC;
		}

		TextureSampler(Framebuffer* framebuffer) : UniformDescriptor(UniformDescriptorType::TextureSampler) {
			m_Framebuffer = framebuffer;
			m_Type = TextureType::FRAMEBUFFER;
		}

		TextureType GetType() { return m_Type; }

		Framebuffer* GetFramebuffer() { return m_Framebuffer; };
		Texture* GetTexture() { return m_Texture; }
	private:
		Texture* m_Texture = nullptr;
		Framebuffer* m_Framebuffer = nullptr;
		TextureType m_Type;
	};

	class UniformBuffer
	{
	public:
		virtual void WriteData(void* data, uint32_t size) = 0;
		// -- Write a uniform object
		virtual void WriteData(UniformObject* object) = 0;
		static UniformBuffer* Create(std::initializer_list<UniformDescriptor*> descriptors);
	private:

	};
}
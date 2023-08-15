#include "RendererAPI.h"
#include "Texture.h"

#include "Arcane/Platform/Vulkan/VulkanTexture.h"
#include "Arcane/Platform/OpenGL/OpenGLTexture.h"


namespace Arcane {
	Texture* Texture::Create(std::string fileName) {
		switch (RendererAPI::Current()) 
		{
		case RendererAPIType::Vulkan: return new VulkanTexture(fileName);
		case RendererAPIType::OpenGL: return new OpenGLTexture(fileName);
		default:
			return nullptr;
		}
	}

	Texture* Texture::Create(float r, float g, float b, float a) {
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanTexture(r, g, b, a);
		default:
			return nullptr;
		}
	}

	Texture* Texture::Create(char* data, uint32_t size, uint32_t width, uint32_t height)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanTexture(data, size, width, height);
		default: return nullptr;
		}
	}
}
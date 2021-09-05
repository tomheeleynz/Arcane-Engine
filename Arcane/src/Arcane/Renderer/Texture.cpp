#include "RendererAPI.h"
#include "Texture.h"

#include "Arcane/Platform/Vulkan/VulkanTexture.h"


namespace Arcane {
	Texture* Texture::Create(std::string fileName) {
		switch (RendererAPI::Current()) 
		{
		case RendererAPIType::Vulkan: return new VulkanTexture(fileName);
		default:
			return nullptr;
		}
	}
}
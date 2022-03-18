#include "RendererAPI.h"
#include "Texture.h"

#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanTexture.h"
#endif

namespace Arcane {
	Texture* Texture::Create(std::string fileName) {
		switch (RendererAPI::Current()) 
		{
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanTexture(fileName);
#endif
		default:
			return nullptr;
		}
	}
}

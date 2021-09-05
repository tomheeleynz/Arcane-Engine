#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Arcane/Renderer/Texture.h"

namespace Arcane 
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(std::string fileName);
	private:

	};
}
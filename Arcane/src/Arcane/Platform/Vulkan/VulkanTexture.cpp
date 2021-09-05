#include "VulkanTexture.h"

namespace Arcane
{
	VulkanTexture::VulkanTexture(std::string fileName)
	{
		int width, height, channels = 0;
		stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels) {
			printf("Data loaded\n");
		}
	}
}
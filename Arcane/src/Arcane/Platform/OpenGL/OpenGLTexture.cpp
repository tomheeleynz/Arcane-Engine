#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "OpenGLTexture.h"

namespace Arcane
{
	OpenGLTexture::OpenGLTexture(std::string filename)
	{
		int width, height, channels = 0;
		stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels != nullptr) 
		{
				
		}
	}

	void OpenGLTexture::UpdateTexture(std::string filename)
	{

	}

	void OpenGLTexture::UpdateTexture(Texture* texture)
	{

	}

	void OpenGLTexture::UpdateTexture(float r, float g, float b, float a)
	{

	}
}
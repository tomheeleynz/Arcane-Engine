#include <stb_image.h>
#include <glad/glad.h>

#include "OpenGLTexture.h"

namespace Arcane
{
	OpenGLTexture::OpenGLTexture(std::string filename)
	{
		int width, height, channels = 0;
		stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels != nullptr) {
			glGenTextures(1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture\n";
		}

		stbi_image_free(pixels);
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
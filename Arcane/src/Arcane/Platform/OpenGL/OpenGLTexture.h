#pragma once
#include "Arcane/Renderer/Texture.h"

namespace Arcane
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(std::string filename);

		void UpdateTexture(std::string filename) override;
		void UpdateTexture(Texture* texture) override;
		void UpdateTexture(float r, float g, float b, float a) override;
	private:
	
	};
}
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

		TextureImageDataType GetTextureDataType() override;
		void SetTextureDataType(TextureImageDataType type) override;

		glm::vec4 GetRGBAValue() override;
		void SetRGBAValue(glm::vec4 value) override;

		uint32_t GetTextureID() { return m_TextureID; }
	private:
		uint32_t m_TextureID;
		TextureImageDataType m_TextureDataType;
		glm::vec4 m_RGBAValue;
	};
}
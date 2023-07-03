#pragma once

#include "Arcane/Assets/Asset.h"

#include <string>
#include <glm/glm.hpp>

namespace Arcane {
	enum class TextureImageDataType
	{
		RGBA,
		SAMPLER
	};

	class Texture : public Asset
	{
	public:
		virtual void UpdateTexture(std::string filename) = 0;
		virtual void UpdateTexture(Texture* texture) = 0;
		virtual void UpdateTexture(float r, float g, float b, float a) = 0;

		virtual TextureImageDataType GetTextureDataType() = 0;
		virtual void SetTextureDataType(TextureImageDataType type) = 0;

		static Texture* Create(std::string fileName);
		static Texture* Create(float r, float g, float b, float a);

		virtual glm::vec4 GetRGBAValue() = 0;
		virtual void SetRGBAValue(glm::vec4 value) = 0;
	private:
		
	};
}
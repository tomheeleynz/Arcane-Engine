#pragma once

#include <string>
namespace Arcane {
	class Texture
	{
	public:
		virtual void UpdateTexture(std::string filename) = 0;
		virtual void UpdateTexture(Texture* texture) = 0;
		virtual void UpdateTexture(int data) = 0;

		static Texture* Create(std::string fileName);
		static Texture* Create(float r, float g, float b, float a);
	private:

	};
}
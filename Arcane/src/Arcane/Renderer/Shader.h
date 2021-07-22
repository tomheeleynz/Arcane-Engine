#pragma once

#include <string>

namespace Arcane {
	class Shader
	{
	public:

		static Shader* Create(std::string vertexShader, std::string fragmentShader);
	private:

	};
}
#pragma once

#include "Shader.h"

namespace Arcane
{
	class Material
	{
	public:
		virtual std::vector<ShaderVariable> GetShaderVariables() = 0;
		static Material* Create(Shader* shader);
	private:
	};
}
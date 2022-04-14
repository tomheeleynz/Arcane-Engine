#pragma once

#include "Shader.h"

namespace Arcane
{
	class Material
	{
	public:
		static Material* Create(Shader* shader);

		// Virtual Methods
		virtual Shader* GetShader() = 0;
		virtual void SetShader(Shader* shader) = 0;
	private:
	};
}
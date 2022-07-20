#pragma once

#include "Shader.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"

namespace Arcane
{
	class Material
	{
	public:
		virtual std::vector<ShaderVariable> GetShaderVariables() = 0;
		virtual DescriptorSet* GetDescriptorSet() = 0;
		virtual UniformBuffer* GetUniformBuffer() = 0;
		

		static Material* Create(Shader* shader);
	private:
	};
}
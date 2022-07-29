#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"

namespace Arcane
{
	class Material
	{
	public:
		virtual std::vector<ShaderVariable> GetMaterialVariables() = 0;
		virtual DescriptorSet* GetDescriptorSet() = 0;
		virtual UniformBuffer* GetUniformBuffer() = 0;

		// Get Material Variables from material buffer
		virtual glm::vec3 GetVec3(uint32_t offset) = 0;
		
		// Write Material Variables to material buffer
		virtual void WriteVec3(uint32_t offset, glm::vec3 value) = 0;

		static Material* Create(Shader* shader);
	private:
	};
}
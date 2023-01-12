#pragma once
#include <glm/glm.hpp>

#include "Arcane/Assets/Asset.h"
#include "Shader.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"

namespace Arcane
{
	class Material : public Asset
	{
	public:
		virtual std::vector<ShaderVariable> GetMaterialVariables() = 0;
		virtual DescriptorSet* GetDescriptorSet() = 0;
		virtual UniformBuffer* GetUniformBuffer() = 0;

		// Get Material Variables from material buffer
		virtual glm::vec3 GetVec3(int binding, uint32_t offset) = 0;
		virtual Texture* GetTexture(int binding) = 0;

		// Write Material Variables to material buffer
		virtual void WriteVec3(int binding, uint32_t offset, glm::vec3 value) = 0;

		// Write Texture (both image and color)
		virtual void WriteTexture(int binding, Texture* texture) = 0;
		virtual void WriteTexture(int binding, glm::vec4 color) = 0;

		// Update Material data from internal structures
		virtual void UpdateMaterialData() = 0;

		// Get the shader from the material



		static Material* Create(Shader* shader);
	private:
	};
}
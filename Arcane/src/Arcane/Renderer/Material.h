#pragma once
#include <glm/glm.hpp>

#include "Arcane/Assets/Asset.h"
#include "Shader.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"
#include "Pipeline.h"

namespace Arcane
{
	// Struct to define each one of the descriptors a material holds
	// This will reflect all of them, but stuff like global data or per object data wont be
	// able to be accessed, and only the set 2 will be exposed
	// as that is the set

	class Material : public Asset
	{
	public:
		virtual std::vector<ShaderSet> GetShaderSets() = 0;
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
		virtual Shader* GetShader() = 0;
		virtual void SetShader(Shader* shader) = 0;

		static Material* Create(Shader* shader);
	private:
	};
}
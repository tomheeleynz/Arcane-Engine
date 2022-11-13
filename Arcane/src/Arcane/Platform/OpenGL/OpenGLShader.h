#pragma once
#include "Arcane/Renderer/Shader.h"

#include <string>
#include <fstream>
#include <spirv_glsl.hpp>

namespace Arcane 
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string vertexShader, std::string fragmentShader);

		DescriptorSet* GetMaterialDescriptor() override;
		std::vector<ShaderVariable> GetMaterialVariables() override;
		uint32_t GetMaterialSize() override;
	private:
		uint32_t m_ShaderProgram;
	};
}
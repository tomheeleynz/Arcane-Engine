#pragma once

#include <string>
#include <map>
#include <vector>

#include "Arcane/Assets/Asset.h"
#include "DescriptorSet.h"

namespace Arcane 
{
	enum class ShaderVariableType
	{
		Float,
		Vec3,
		Vec2,
		Int,
		Sampler
	};

	struct ShaderVariable
	{
		std::string Name;
		ShaderVariableType Type;
		uint32_t size;
		uint32_t offset;
		uint32_t binding;
	};

	/////////////////////////////////////////////////////////////
	//// Shader
	/////////////////////////////////////////////////////////////
	class Shader : public Asset
	{
	public:
		virtual DescriptorSet* GetMaterialDescriptor() = 0;
		virtual std::vector<ShaderVariable> GetMaterialVariables() = 0;
		virtual uint32_t GetMaterialSize() = 0;
		static Shader* Create(std::string vertexShader, std::string fragmentShader);
	private:

	};

	/////////////////////////////////////////////////////////////
	//// Shader Library
	/////////////////////////////////////////////////////////////
	class ShaderLibrary
	{
	public:
		static Shader* GetShader(std::string shaderName);
		static ShaderLibrary* GetInstance();
	private:
		ShaderLibrary();
		static ShaderLibrary* s_Instance;
		Shader* GetShaderImpl(std::string shaderName);

	private:
		std::map<std::string, Shader*> m_Shaders;

	};
}
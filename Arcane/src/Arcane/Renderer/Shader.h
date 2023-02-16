#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "Arcane/Assets/Asset.h"
#include "DescriptorSet.h"

namespace Arcane
{
	enum class ShaderVariableLocation
	{
		VERTEX,
		FRAGMENT
	};

	enum class ShaderBindingType
	{
		SAMPLER,
		UNIFORM
	};

	enum class ShaderMemberType
	{
		Vec4,
		Vec3,
		Vec2,
		Mat4,
		Mat3
	};

	struct ShaderMember
	{
		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		ShaderMemberType type;
		std::vector<ShaderMember> Members;
	};

	struct ShaderBinding
	{
		uint32_t Binding;
		uint32_t Size;
		uint32_t Offset;
		ShaderBindingType Type;
		std::vector<ShaderMember> Members;
	};

	struct ShaderSet
	{
		std::string Name;
		uint32_t SetNumber;
		uint32_t Size;
		std::vector<ShaderBinding> Bindings;
	};

	struct ShaderProgramSource
	{
		std::string vertexShader;
		std::string fragmentShader;
	};

	/////////////////////////////////////////////////////////////
	//// Shader
	/////////////////////////////////////////////////////////////
	class Shader : public Asset
	{
	public:
		virtual DescriptorSet* GetMaterialDescriptor() = 0;
		virtual std::vector<ShaderSet> GetShaderSets() = 0;
		virtual uint32_t GetMaterialSize() = 0;

		static ShaderProgramSource ParseShader(std::string shaderFile);
		static Shader* Create(std::string vertexShader, std::string fragmentShader);
		static Shader* Create(std::string shaderFile);
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
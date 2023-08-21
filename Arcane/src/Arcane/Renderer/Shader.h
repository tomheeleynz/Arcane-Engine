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
		uint32_t DescriptorCount;
		ShaderBindingType Type;
		ShaderVariableLocation Location;
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
		static Shader* Create(uint32_t* vertexByteCode, uint32_t vertexBytesSize, uint32_t* fragmentByteCode, uint32_t fragmentBytesSize);
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

		static void AddShader(std::string name, Shader* shader);
		static void PackShaderLibrary(std::ofstream& o);
	private:
		ShaderLibrary();
		static ShaderLibrary* s_Instance;
		Shader* GetShaderImpl(std::string shaderName);

		void AddShaderImpl(std::string name, Shader* shader);
		void PackShaderLibraryImpl(std::ofstream& o);
	private:
		std::map<std::string, Shader*> m_Shaders;

	};
}
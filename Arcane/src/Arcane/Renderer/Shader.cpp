#include "Shader.h"
#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Platform/Vulkan/VulkanShader.h"

namespace Arcane 
{
	/////////////////////////////////////////////////////////////
	//// Shader
	/////////////////////////////////////////////////////////////
	Shader* Shader::Create(std::string vertexShader, std::string fragmentShader)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanShader(vertexShader, fragmentShader);
		default: return nullptr;
		}
	}

	/////////////////////////////////////////////////////////////
	//// Shader Library
	/////////////////////////////////////////////////////////////
	ShaderLibrary* ShaderLibrary::s_Instance = nullptr;

	ShaderLibrary::ShaderLibrary()
	{
		// Load Shaders
		m_Shaders["Mesh"] = Shader::Create(
			".\\src\\Assets\\Shaders\\MeshVert.spv",
			".\\src\\Assets\\Shaders\\MeshFrag.spv"
		);
		
		m_Shaders["Grid"] = Shader::Create(
			".\\src\\Assets\\Shaders\\GridVert.spv",
			".\\src\\Assets\\Shaders\\GridFrag.spv"
		);
		
		m_Shaders["Screen"] = Shader::Create(
			".\\src\\Assets\\Shaders\\ScreenVert.spv",
			".\\src\\Assets\\Shaders\\ScreenFrag.spv"
		);
	}

	ShaderLibrary* ShaderLibrary::GetInstance()
	{
		if (!s_Instance)
			s_Instance = new ShaderLibrary();

		return s_Instance;
	}

	Shader* ShaderLibrary::GetShader(std::string shaderName)
	{
		return GetInstance()->GetShaderImpl(shaderName);
	}

	Shader* ShaderLibrary::GetShaderImpl(std::string shaderName)
	{
		return m_Shaders[shaderName];
	}
}

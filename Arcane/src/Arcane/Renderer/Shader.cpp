#include "Shader.h"
#include "Arcane/Renderer/Renderer.h"

// -- Vulkan Shader
#include "Arcane/Platform/Vulkan/VulkanShader.h"

// -- OpenGL Shader
#include "Arcane/Platform/OpenGL/OpenGLShader.h"

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
		case RendererAPIType::OpenGL: return new OpenGLShader(vertexShader, fragmentShader);
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
			".\\src\\EditorAssets\\Shaders\\MeshVert.spv",
			".\\src\\EditorAssets\\Shaders\\MeshTextured.spv"
		);
		
		m_Shaders["Grid"] = Shader::Create(
			".\\src\\EditorAssets\\Shaders\\GridVert.spv",
			".\\src\\EditorAssets\\Shaders\\GridFrag.spv"
		);
		
		m_Shaders["Screen"] = Shader::Create(
			".\\src\\EditorAssets\\Shaders\\ScreenVert.spv",
			".\\src\\EditorAssets\\Shaders\\ScreenFrag.spv"
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

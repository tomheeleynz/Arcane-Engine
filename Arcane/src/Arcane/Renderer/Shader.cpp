#include "Shader.h"
#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Core/Application.h"

// -- Vulkan Shader
#include "Arcane/Platform/Vulkan/VulkanShader.h"

// -- OpenGL Shader
#include "Arcane/Platform/OpenGL/OpenGLShader.h"

namespace Arcane 
{
	ShaderProgramSource Shader::ParseShader(std::string shaderFile)
	{
		std::ifstream stream(shaderFile);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line)) 
		{
			if (line.find("#shader") != std::string::npos) 
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}


		return { ss[0].str(), ss[1].str() };
	}

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

	Shader* Shader::Create(std::string shaderFile)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanShader(shaderFile);
		case RendererAPIType::OpenGL: return new OpenGLShader(shaderFile);
		default: return nullptr;
		}
	}

	/////////////////////////////////////////////////////////////
	//// Shader Library
	/////////////////////////////////////////////////////////////
	ShaderLibrary* ShaderLibrary::s_Instance = nullptr;

	ShaderLibrary::ShaderLibrary()
	{
		std::string dir = std::string(Application::Get().GetEditorAssetPath());

		// Load Shaders
		m_Shaders["Mesh"] = Shader::Create(
			dir + "\\src\\EditorAssets\\Shaders\\MeshVert.spv",
			dir + "\\src\\EditorAssets\\Shaders\\MeshTextured.spv"
		);
		
		m_Shaders["Grid"] = Shader::Create(
			dir + "\\src\\EditorAssets\\Shaders\\GridVert.spv",
			dir + "\\src\\EditorAssets\\Shaders\\GridFrag.spv"
		);
		
		m_Shaders["Screen"] = Shader::Create(
			dir + "\\src\\EditorAssets\\Shaders\\ScreenVert.spv",
			dir + "\\src\\EditorAssets\\Shaders\\ScreenFrag.spv"
		);

		m_Shaders["Sprite-Default"] = Shader::Create(dir + "\\src\\EditorAssets\\Shaders\\Sprite.arcaneshader");
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

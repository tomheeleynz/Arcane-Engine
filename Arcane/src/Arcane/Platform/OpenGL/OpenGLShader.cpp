#include "OpenGLShader.h"

#include <glad/glad.h>
#include <iostream>

namespace Arcane
{
	static std::vector<uint32_t> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			printf("File Not Open\n");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		std::vector<uint32_t> test;
		for (int i = 0; i < buffer.size() - 3; i += 4) {
			uint32_t num = (uint32_t)buffer[i] << 24 | 
					(uint32_t)buffer[i + 1] << 16 |	
					(uint32_t)buffer[i + 2] << 8 | 
					(uint32_t)buffer[i + 3];

			test.push_back(num);
		}

		return test;
	}

	OpenGLShader::OpenGLShader(std::string vertexShader, std::string fragmentShader)
	{
		GLuint vertexProgram = 0;
		// Setup Vertex Source
		{
			auto vertexSpirv = readFile(vertexShader);
			spirv_cross::CompilerGLSL glsl(std::move(vertexSpirv));
			std::string source = glsl.compile();
			std::cout << source << std::endl;

			GLuint vertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);
			const GLchar* shaderSource = (const GLchar*)source.c_str();
			
			glShaderSource(vertexShaderProgram, 1, &shaderSource, 0);
			glCompileShader(vertexShaderProgram);

			GLint isCompiled = 0;
			glGetShaderiv(vertexShaderProgram, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(vertexShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(vertexShaderProgram, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(vertexShaderProgram);
			}
			else {
				vertexProgram = vertexShaderProgram;
			}
		}

		std::cout << std::endl;

		GLuint fragmentProgram = 0;
		// Setup Fragment Source 
		{
			auto fragmentSpirv = readFile(fragmentShader);
			spirv_cross::CompilerGLSL glsl(std::move(fragmentSpirv));
			std::string source = glsl.compile();
			std::cout << source << std::endl;

			GLuint fragmentShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
			const GLchar* shaderSource = (const GLchar*)source.c_str();

			glShaderSource(fragmentShaderProgram, 1, &shaderSource, 0);
			glCompileShader(fragmentShaderProgram);

			GLint isCompiled = 0;
			glGetShaderiv(fragmentShaderProgram, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(fragmentShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(fragmentShaderProgram, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(fragmentShaderProgram);
			}
			else {
				fragmentProgram = fragmentShaderProgram;
			}
		}

		// Link Program
		m_ShaderProgram = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ShaderProgram, vertexProgram);
		glAttachShader(m_ShaderProgram, fragmentProgram);

		// Link our program
		glLinkProgram(m_ShaderProgram);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ShaderProgram, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_ShaderProgram);

			glDeleteShader(vertexProgram);
			glDeleteShader(fragmentProgram);
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_ShaderProgram, vertexProgram);
		glDetachShader(m_ShaderProgram, fragmentProgram);

	}

	DescriptorSet* OpenGLShader::GetMaterialDescriptor()
	{
		return nullptr;
	}

	std::vector<ShaderVariable> OpenGLShader::GetMaterialVariables()
	{
		return std::vector<ShaderVariable>();
	}

	uint32_t OpenGLShader::GetMaterialSize()
	{
		return uint32_t();
	}
	
	void OpenGLShader::Bind()
	{
		glUseProgram(m_ShaderProgram);
	}

	void OpenGLShader::UnBind()
	{
		glUseProgram(0);
	}

}
#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Arcane
{
	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			printf("File Not Open\n");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	OpenGLShader::OpenGLShader(std::string vertexShader, std::string fragmentShader)
	{
		// Vertex Shader
		{
			auto vertexSpirv = readFile(vertexShader);
			uint32_t vertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);
			glShaderBinary(1, &vertexShaderProgram, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexSpirv.data(), vertexSpirv.size());
			std::string vsEntryPoint = "main";

			glSpecializeShader(vertexShaderProgram, (const GLchar*)vsEntryPoint.c_str(), 0, nullptr, nullptr);

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
		}

		// Fragment Shader
		{
			auto fragmentSpirv = readFile(vertexShader);
			uint32_t fragmentShaderProgram = glCreateShader(GL_VERTEX_SHADER);
			glShaderBinary(1, &fragmentShaderProgram, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentSpirv.data(), fragmentSpirv.size());
			std::string fsEntryPoint = "main";

			glSpecializeShader(fragmentShaderProgram, (const GLchar*)fsEntryPoint.c_str(), 0, nullptr, nullptr);

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
		}
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

}
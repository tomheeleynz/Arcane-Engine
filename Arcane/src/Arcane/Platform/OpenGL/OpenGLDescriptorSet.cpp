#include "OpenGLDescriptorSet.h"
#include "OpenGLTexture.h"

#include<glad/glad.h>

namespace Arcane
{
	OpenGLDescriptorSet::OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs)
	{
		for (DescriptorLayoutSpecs spec : layoutSpecs) {
			m_Textures[{spec.Binding, spec.Name}] = nullptr;
		}
	}

	OpenGLDescriptorSet::OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs)
	{
	}

	void OpenGLDescriptorSet::AddImageSampler(Texture* texture, uint32_t setNum, uint32_t bindingNum)
	{
		std::map<std::pair<int, std::string>, Texture*>::iterator it;
		for (it = m_Textures.begin(); it != m_Textures.end(); it++) {
			if (it->first.first == bindingNum) {
				it->second = texture;
			}
		}
	}

	void OpenGLDescriptorSet::AddImageSampler(Framebuffer* framebuffer, uint32_t setNum, uint32_t bindingNum)
	{
	}

	void OpenGLDescriptorSet::AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum)
	{
	}

	void OpenGLDescriptorSet::BindTextures(uint32_t shaderID)
	{
		int textureCount = 0;

		std::map<std::pair<int, std::string>, Texture*>::iterator it;
		for (it = m_Textures.begin(); it != m_Textures.end(); it++) {
			// Cast Texture to opengl texture
			OpenGLTexture* openglTexture = static_cast<OpenGLTexture*>(it->second);

			// Get Texture Uniform Name
			std::string uniformName = it->first.second;

			// Set Uniform Value
			glUniform1i(glGetUniformLocation(shaderID, uniformName.c_str()), textureCount);
			
			// Set Active Texture
			glActiveTexture(GL_TEXTURE + textureCount);
			glBindTexture(GL_TEXTURE_2D, openglTexture->GetTextureID());
		}
	}
}
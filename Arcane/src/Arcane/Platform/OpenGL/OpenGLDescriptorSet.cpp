#include "OpenGLDescriptorSet.h"
#include "OpenGLTexture.h"
#include "OpenGLFramebuffer.h"

#include<glad/glad.h>

namespace Arcane
{
	OpenGLDescriptorSet::OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs)
	{
		for (DescriptorLayoutSpecs spec : layoutSpecs) {
			DescriptorInfo newInfo;
			newInfo.binding = spec.Binding;
			newInfo.set = setSpecs.SetNumber;
			newInfo.name = spec.Name;

			m_TextureInfo.push_back(newInfo);
		}
	}

	OpenGLDescriptorSet::OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs)
	{
	}

	void OpenGLDescriptorSet::AddImageSampler(Texture* texture, uint32_t setNum, uint32_t bindingNum)
	{
		for (int i = 0; i < m_TextureInfo.size(); i++) {
			DescriptorInfo& currentInfo = m_TextureInfo[i];
			currentInfo.type = TextureSourceType::TEXTURE;
			currentInfo.texture = texture;
		}
	}

	void OpenGLDescriptorSet::AddImageSampler(Framebuffer* framebuffer, uint32_t setNum, uint32_t bindingNum)
	{
		for (int i = 0; i < m_TextureInfo.size(); i++) {
			DescriptorInfo& currentInfo = m_TextureInfo[i];
			currentInfo.type = TextureSourceType::FRAMEBUFFER;
			currentInfo.framebuffer = framebuffer;
		}
	}

	void OpenGLDescriptorSet::AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum)
	{

	}

	void OpenGLDescriptorSet::BindTextures(uint32_t shaderID)
	{
		int textureCount = 0;

		for (int i = 0; i < m_TextureInfo.size(); i++) {
			DescriptorInfo currentInfo = m_TextureInfo[i];

			// Get Texture Uniform Name
			std::string uniformName = currentInfo.name;

			// Set Uniform Value
			glUniform1i(glGetUniformLocation(shaderID, uniformName.c_str()), textureCount);

			uint32_t textureID = -1;

			if (currentInfo.type == TextureSourceType::TEXTURE) {
				textureID = static_cast<OpenGLTexture*>(currentInfo.texture)->GetTextureID();
			}
			else {
				textureID = static_cast<OpenGLFramebuffer*>(currentInfo.framebuffer)->GetFramebufferID();
			}

			glActiveTexture(GL_TEXTURE + textureCount);
			glBindTexture(GL_TEXTURE_2D, textureID);
			textureCount++;
		}
	}
}
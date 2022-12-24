#pragma once

#include <map>
#include "Arcane/Renderer/DescriptorSet.h"

namespace Arcane
{
	class OpenGLDescriptorSet : public DescriptorSet
	{
	public:
		OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::initializer_list<DescriptorLayoutSpecs> layoutSpecs);
		OpenGLDescriptorSet(DescriptorSetSpecs& setSpecs, std::vector<DescriptorLayoutSpecs> layoutSpecs);


		void AddImageSampler(Texture* texture, uint32_t setNum, uint32_t bindingNum) override;
		void AddImageSampler(Framebuffer* framebuffer, uint32_t setNum, uint32_t bindingNum) override;
		void AddUniformBuffer(UniformBuffer* buffer, uint32_t setNum, uint32_t bindingNum) override;

		// OpenGL Specific Functions
		void BindTextures(uint32_t shaderID);
	private:
		enum class TextureSourceType
		{
			TEXTURE,
			FRAMEBUFFER
		};

		struct DescriptorInfo 
		{
			int set;
			int binding;
			std::string name;
			TextureSourceType type;
			Texture* texture = nullptr;
			Framebuffer* framebuffer = nullptr;
		};
	private:
		std::map<std::pair<int, std::string>, Texture*> m_Textures;
		std::vector<DescriptorInfo> m_TextureInfo;
	};
}
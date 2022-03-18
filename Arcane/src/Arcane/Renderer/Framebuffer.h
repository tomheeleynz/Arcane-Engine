#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <initializer_list>

namespace Arcane {

	enum class FramebufferAttachmentType
	{
		COLOR, 
		DEPTH
	};

	struct FrameBufferAttachmentSpecs 
	{
		FrameBufferAttachmentSpecs() = default;
		FrameBufferAttachmentSpecs(std::initializer_list<FramebufferAttachmentType> attachments)
			: m_Attachments(attachments) {}

		std::initializer_list<FramebufferAttachmentType> m_Attachments;
	};

	struct FramebufferSpecifications
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		glm::vec4 ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		FrameBufferAttachmentSpecs AttachmentSpecs;
	};

	class Framebuffer
	{
	public:
		virtual FramebufferSpecifications GetSpecs() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		static Framebuffer* Create(FramebufferSpecifications& specs);
	private:

	};
}
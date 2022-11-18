#pragma once
#include <Arcane.h>

class OpenGLTestLayer : public Arcane::Layer
{
public:
	OpenGLTestLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
private:
	Arcane::Framebuffer* m_Framebuffer;
	Arcane::RenderPass* m_RenderPass;
	Arcane::VertexBuffer* m_VertexBuffer;
	Arcane::IndexBuffer* m_IndexBuffer;
	Arcane::Shader* m_Shader;
	Arcane::VertexDescriptor* m_VertexDescriptor;
	Arcane::Pipeline* m_Pipeline;
};
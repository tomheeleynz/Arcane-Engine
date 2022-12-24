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
	// Geo Pass
	Arcane::Framebuffer* m_Framebuffer;
	Arcane::RenderPass* m_RenderPass;
	Arcane::VertexBuffer* m_VertexBuffer;
	Arcane::IndexBuffer* m_IndexBuffer;
	Arcane::Shader* m_Shader;
	Arcane::VertexDescriptor* m_VertexDescriptor;
	Arcane::Pipeline* m_Pipeline;
	Arcane::Texture* m_Texture;
	Arcane::DescriptorSet* m_DescriptorSet;

	// Composite Pass
	Arcane::Framebuffer* m_CompFramebuffer;
	Arcane::RenderPass* m_CompRenderPass;
	Arcane::VertexBuffer* m_CompVertexBuffer;
	Arcane::IndexBuffer* m_CompIndexBuffer;
	Arcane::Shader* m_CompShader;
	Arcane::VertexDescriptor* m_CompVertexDescriptor;
	Arcane::Pipeline* m_CompPipeline;
	Arcane::DescriptorSet* m_CompDescriptorSet;
};
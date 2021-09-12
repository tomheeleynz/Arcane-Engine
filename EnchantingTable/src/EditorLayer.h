#pragma once

#include <Arcane.h>

class EditorLayer : public Arcane::Layer
{
public:
	EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
private:
	// Test Data
	Arcane::Shader* m_Shader;
	Arcane::RenderPass* m_RenderPass;
	Arcane::VertexDescriptor* m_VertexDescriptor;
	Arcane::VertexBuffer* m_VertexBuffer;
	Arcane::Pipeline* m_Pipeline;
	Arcane::UniformBuffer* m_UniformBuffer;
	Arcane::Framebuffer* m_Framebuffer;
};
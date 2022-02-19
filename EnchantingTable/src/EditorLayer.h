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
	// -- Geo Shader
	Arcane::Shader* m_Shader;
	
	// -- Screen Shader
	Arcane::Shader* m_ScreenSpaceShader;

	Arcane::RenderPass* m_RenderPass;
	
	Arcane::VertexDescriptor* m_VertexDescriptor;
	Arcane::VertexBuffer* m_VertexBuffer;
	Arcane::Pipeline* m_Pipeline;


	// Test Uniform Buffer
	Arcane::UniformBuffer* m_UniformBuffer;
	Arcane::UniformObject* m_ColorObject;

	Arcane::Framebuffer* m_Framebuffer;
	Arcane::Texture* m_Texture;
};
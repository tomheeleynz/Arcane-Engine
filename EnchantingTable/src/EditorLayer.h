#pragma once

#include <imgui.h>
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
	// Screen Renderpasss
	Arcane::Shader* m_ScreenSpaceShader;
	Arcane::RenderPass* m_ScreenRenderPass;
	Arcane::Framebuffer* m_ScreenFramebuffer;
	Arcane::Pipeline* m_ScreenPipeline;
	Arcane::VertexDescriptor* m_ScreenVertexDescriptor;
	Arcane::VertexBuffer* m_ScreenVertexBuffer;
	Arcane::IndexBuffer* m_ScreenIndexBuffer;
	Arcane::UniformBuffer* m_ScreenUniformBuffer;
	Arcane::TextureSampler* m_FramebufferSampler;

	// Test Data
	// -- Geo Shader
	Arcane::Shader* m_Shader;
	
	// -- Screen Shader

	Arcane::RenderPass* m_RenderPass;
	
	Arcane::VertexDescriptor* m_VertexDescriptor;
	Arcane::VertexBuffer* m_VertexBuffer;
	Arcane::Pipeline* m_Pipeline;

	// Test Uniform Buffer
	Arcane::UniformBuffer* m_UniformBuffer;
	Arcane::UniformObject* m_ColorObject;
	Arcane::TextureSampler* m_TestSampler;

	Arcane::Framebuffer* m_Framebuffer;
	Arcane::Texture* m_Texture;

	ImTextureID m_Viewport;
};
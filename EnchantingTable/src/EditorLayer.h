#pragma once

#include <imgui.h>
#include <Arcane.h>
#include <glm/glm.hpp>


// Panels
#include "Panels/ScenePanel.h"

class EditorLayer : public Arcane::Layer
{
public:
	EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;

	void OpenScene();
	void SaveScene();
private:
	Arcane::Framebuffer* m_ScreenFramebuffer;
	Arcane::Shader* m_Shader;
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

	// Viewport Window
	ImTextureID m_Viewport;
	glm::vec2 m_ViewportSize;

	// Scene
	Arcane::Scene* m_ActiveScene;

	// Panels
	ScenePanel* m_ScenePanel;
};
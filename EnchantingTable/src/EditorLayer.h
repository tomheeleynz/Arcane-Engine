#pragma once

#include <imgui.h>
#include <Arcane.h>
#include <glm/glm.hpp>

// Panels
#include "Panels/ScenePanel.h"
#include "Panels/EntityPanel.h"
#include "Panels/FileBrowser.h"
#include "Panels/MaterialViewerPanel.h"
#include "Controllers/PerspectiveController.h"

class EditorLayer : public Arcane::Layer
{
public:
	enum class SceneState
	{
		EDIT,
		PLAY,
		PAUSE
	};

	EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;

	void OpenScene();
	void SaveScene();
private:
	// Viewport Window
	ImTextureID m_Viewport;
	glm::vec2 m_ViewportSize;

	// Scene
	Arcane::Scene* m_ActiveScene;
	Arcane::SceneRenderer* m_SceneRenderer;

	// Panels
	ScenePanel* m_ScenePanel;
	EntityPanel* m_EntityPanel;
	FileBrowserPanel* m_FileBrowserPanel;
	MaterialViewerPanel* m_MaterialViewerPanel;

	// Editor Camera
	Arcane::PerspectiveCamera* m_EditorCamera;
	PerspectiveController* m_EditorCameraController;

	// Scene State
	SceneState m_State = SceneState::EDIT;
};
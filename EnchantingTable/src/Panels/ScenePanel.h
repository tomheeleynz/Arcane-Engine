#pragma once

#include <Arcane.h>
#include <imgui.h>

class ScenePanel
{
public:
	ScenePanel();

	void SetContext(Arcane::Scene* context);
	void Update();

	void DrawNode(Arcane::Entity& entity);

	Arcane::Entity& GetSelectedEntity() { return m_SelectedEntity; }
private:
	Arcane::Scene* m_Context = nullptr;
	Arcane::Entity m_SelectedEntity;
};
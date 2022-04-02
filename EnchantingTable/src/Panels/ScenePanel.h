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
private:
	Arcane::Scene* m_Context = nullptr;
};
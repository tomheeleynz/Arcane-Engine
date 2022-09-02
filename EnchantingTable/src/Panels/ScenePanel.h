#pragma once

#include <Arcane.h>
#include <imgui.h>

class ScenePanel
{
public:
	enum class MeshEntityType
	{
		PLANE,
		CUBE
	};

	ScenePanel();

	void SetContext(Arcane::Scene* context);
	void Update();

	void DrawNode(Arcane::Entity& entity);

	Arcane::Entity& GetSelectedEntity() { return m_SelectedEntity; }
private:
	// Creation Function from popup menu
	Arcane::Entity& CreateMeshEntity(MeshEntityType type);

private:
	Arcane::Scene* m_Context = nullptr;
	Arcane::Entity m_SelectedEntity;
};
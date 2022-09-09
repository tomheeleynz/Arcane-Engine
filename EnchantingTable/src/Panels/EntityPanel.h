#pragma once

#include <Arcane.h>
#include <glm/gtc/type_ptr.hpp>

class EntityPanel
{
public:
	EntityPanel();

	void SetContext(Arcane::Entity& entity);

	void Update();
	void DrawComponents(Arcane::Entity& entity);

	Arcane::Material* GetSelectedMaterial() { return m_SelectedMaterial; }
private:
	void AddMeshComponent(Arcane::Entity& entity);

private:
	Arcane::Entity m_Context;
	Arcane::Material* m_SelectedMaterial = nullptr;
};
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

private:
	void AddMeshComponent(Arcane::Entity& entity);

private:
	Arcane::Entity m_Context;
};
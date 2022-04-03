#pragma once

#include <Arcane.h>

class EntityPanel
{
public:
	EntityPanel();

	void SetContext(Arcane::Entity& entity);

	void Update();
	void DrawComponents(Arcane::Entity& entity);
private:
	Arcane::Entity m_Context;
};
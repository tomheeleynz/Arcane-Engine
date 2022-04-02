#pragma once

#include <Arcane.h>

class EntityPanel
{
public:
	EntityPanel();

	void SetContext(Arcane::Entity& entity);

	void Update();
private:
	Arcane::Entity m_Context;
};
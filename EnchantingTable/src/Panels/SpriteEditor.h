#pragma once

#include <Arcane.h>
#include <imgui.h>

class SpriteEditor
{
public:
	SpriteEditor();

	void OnImGuiRender();
private:
	Arcane::Texture* m_Texture = nullptr;
};
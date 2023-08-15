#pragma once

#include <Arcane.h>
#include <imgui.h>
#include <vector>

struct TextureSpriteRect
{
	float x;
	float y;
	float width;
	float height;
};

class SpriteEditor
{
public:
	SpriteEditor();

	void OnImGuiRender();
private:
	Arcane::Texture* m_Texture = nullptr;
	nlohmann::json m_TextureMetaFile;

private:
	void ApplyMetaInfo();
	std::vector<TextureSpriteRect> m_TextureSpriteRects;
};
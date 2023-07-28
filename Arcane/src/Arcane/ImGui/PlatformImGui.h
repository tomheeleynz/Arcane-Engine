#pragma once

#include <imgui.h>
#include "Arcane/Renderer/Framebuffer.h"
#include "Arcane/Renderer/Texture.h"

namespace Arcane::UI
{
	//////////////////////////////////////////
	/// Images
	//////////////////////////////////////////
	void Image(ImTextureID id, ImVec2 size);
	void Image(Texture* texture, ImVec2 size = ImVec2(128, 128));

	//////////////////////////////////////////
	/// Image Buttons
	//////////////////////////////////////////
	bool ImageButton(ImTextureID id, ImVec2 size);
	bool ImageButton(Texture* texture, ImVec2 size);



	ImTextureID AddTexture(Texture* texture);
	ImTextureID AddTexture(Framebuffer* frameBuffer);
}
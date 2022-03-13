#pragma once

#include <imgui.h>
#include "Arcane/Renderer/Framebuffer.h"
#include "Arcane/Renderer/Texture.h"

namespace Arcane::UI
{
	void Image(ImTextureID id, ImVec2 size);
	void Image(Texture* texture);
	
	ImTextureID AddTexture(Texture* texture);
	ImTextureID AddTexture(Framebuffer* frameBuffer);
}
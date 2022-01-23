#pragma once

#include "Arcane/Renderer/Framebuffer.h"
#include "Arcane/Renderer/Texture.h"

namespace Arcane::UI
{
	void Image(Framebuffer* frameBuffer);	
	void Image(Texture* texture);
}
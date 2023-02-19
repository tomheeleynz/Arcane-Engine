#pragma once

#include "Arcane/Renderer/GraphicsContext.h"

namespace Arcane
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext();

		void SwapBuffers() override;
		void Resize(bool resize) override;
	private:
	};
}
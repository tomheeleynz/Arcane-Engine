#pragma once

#include "Arcane/Core/Window.h"

namespace Arcane {
	class GraphicsContext
	{
	public:
		virtual void Resize(bool resize) = 0;
		virtual void SwapBuffers() = 0;
		static GraphicsContext* Create();
	private:

	};
}
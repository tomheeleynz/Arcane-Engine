#pragma once

#include "Core/Window.h"

namespace Arcane {
	class GraphicsContext
	{
	public:
		virtual void SwapBuffers() = 0;
		static GraphicsContext* Create();
	private:

	};
}
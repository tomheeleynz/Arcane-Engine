#pragma once

#include "Core/Window.h"

class GraphicsContext
{
public:
	virtual void SwapBuffers() = 0;
	static GraphicsContext* Create();
private:

};
#pragma once

#include "RendererAPI.h"

namespace Arcane
{
	#if defined(ARCANE_WIN32)	
		#define VK_USE_PLATFORM_WIN32_KHR
		#include <GLFW/glfw3.h>
		#define GLFW_EXPOSE_NATIVE_WIN32
		#include <GLFW/glfw3native.h>
	#endif

	#if defined(ARCANE_LINUX)
		#include <GLFW/glfw3.h>
		#define GLFW_EXPOSE_NATIVE_X11
		#include <GLFW/glfw3native.h>
	#endif
}

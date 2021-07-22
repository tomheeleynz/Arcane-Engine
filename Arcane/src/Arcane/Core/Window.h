#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace Arcane
{
	struct WindowSpecifications
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
	};

	class GraphicsContext;

	class Window
	{
	public:
		Window(WindowSpecifications& specifications);

		void ProcessEvents();
		void SwapBuffers();

		void* GetNativeWindow();
		void Init();

		GraphicsContext* GetContext() { return m_Context; }
	private:
		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
		};

		WindowData m_Data;
		GLFWwindow* m_Window;

		GraphicsContext* m_Context;
	};
}
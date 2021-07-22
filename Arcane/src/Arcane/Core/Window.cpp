#include "Window.h"
#include "Renderer/GraphicsContext.h"

namespace Arcane {
	Window::Window(WindowSpecifications& specifications)
	{
		// Window Data
		m_Data.Title = specifications.Title;
		m_Data.Width = specifications.Width;
		m_Data.Height = specifications.Height;
	}

	void Window::Init()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = GraphicsContext::Create();
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		m_Context->SwapBuffers();
	}

	void* Window::GetNativeWindow()
	{
		return (void*)m_Window;
	}
}
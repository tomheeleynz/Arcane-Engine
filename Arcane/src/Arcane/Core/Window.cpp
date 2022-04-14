#include "Window.h"
#include "Arcane/Renderer/GraphicsContext.h"
#include "InputManager.h"

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

		// Mouse Coords
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			InputManager::SetMouseCoords((float)xpos, (float)ypos);
		});

		// Key callback
		// Window Key Down Key Press
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action)
			{
			case GLFW_PRESS: {
				InputManager::SetKeyPressed(key);
				break;
			}
			case GLFW_RELEASE: {
				InputManager::SetKeyReleased(key);
				break;
			}
			default:
				break;
			}
		});


		// Mouse Callback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				if (action == GLFW_PRESS) {
					InputManager::SetMouseKeyPressed(button);
				}
				else {
					InputManager::SetMouseKeyReleased(button);
				}
				break;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				if (action == GLFW_PRESS) {
					InputManager::SetMouseKeyPressed(button);
				}
				else {
					InputManager::SetMouseKeyReleased(button);
				}
				break;
			}
			default:
				break;
			}
		});

		// Scroll Callback
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			InputManager::SetScrollOffsets((float)xoffset, (float)yoffset);

		});
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
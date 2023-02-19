#include "OpenGLContext.h"
#include "Arcane/Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Arcane
{
	OpenGLContext::OpenGLContext()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "FAILED TO INIT GLAD\n";
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSwapBuffers(window);
	}
	void OpenGLContext::Resize(bool resize)
	{
	}
}
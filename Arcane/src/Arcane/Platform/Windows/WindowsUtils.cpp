#include <windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Arcane/Core/Application.h"
#include "Arcane/Utils/Utils.h"


namespace Arcane
{
	namespace FileDialog
	{
		std::string OpenFile()
		{
			Application& app = Application::Get();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
			HWND handle = glfwGetWin32Window(window);

			OPENFILENAME ofn = { 0 };
			TCHAR szFile[260] = { 0 };
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = handle;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All Files\0*.*\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == TRUE)
			{
				return ofn.lpstrFile;
			}

			return "";
		}
	}
}
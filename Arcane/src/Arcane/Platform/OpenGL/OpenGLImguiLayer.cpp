#include "OpenGLImguiLayer.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

namespace Arcane
{
	OpenGLImguiLayer::OpenGLImguiLayer()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking for ImGui

		float fontSize = 18.0f;

		std::string boldFileLocation = std::string(Application::Get().GetEditorAssetPath()) + "\\src\\EditorAssets\\Fonts\\opensans\\OpenSans-Bold.ttf";
		std::string regularFileLocation = std::string(Application::Get().GetEditorAssetPath()) + "\\src\\EditorAssets\\Fonts\\opensans\\OpenSans-Regular.ttf";

		io.Fonts->AddFontFromFileTTF(boldFileLocation.c_str(), fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF(regularFileLocation.c_str(), fontSize);

		Application& app = Application::Get();
		GLFWwindow* windowHandle = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);

		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void OpenGLImguiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGLImguiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void OpenGLImguiLayer::OnAttach()
	{

	}
}
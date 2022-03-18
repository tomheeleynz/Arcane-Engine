#pragma once 

#include <string>
#include <memory>
#include <vector>

#include "Clock.h"
#include "Window.h"
#include "Arcane/ImGui/ImGuiLayer.h"


namespace Arcane {

	struct ApplicationSpecifications
	{
		std::string Name;
		uint32_t WindowWidth;
		uint32_t WindowHeight;
		bool PushImguiLayer = false;
	};

	class Application
	{
	public:
		Application(ApplicationSpecifications& specifications);
		~Application();

		void Run();

		// Get Reference To Window
		Window& GetWindow();

		// Get Static Application Reference
		static Application& Get();

		// Push Layer
		void PushLayer(Layer* _layer);

		// ImGui
		void RenderImGui();
	private:
		Window* m_Window;
		static Application* s_Instance;
		std::vector<Layer*> m_LayerStack;
		bool m_bIsRunning = true;
		ImGuiLayer* m_ImGuiLayer;
		CClock* m_Clock;
		bool m_ImguiEnabled = false;
	};
}

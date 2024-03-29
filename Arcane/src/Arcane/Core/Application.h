#pragma once 

#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include <vector>

#include "Clock.h"
#include "Window.h"
#include "Arcane/ImGui/ImGuiLayer.h"
#include "Arcane/Assets/AssetDatabase.h"
#include "Arcane/Project/ProjectDeserializer.h"
#include "Arcane/Physics/PhysicsEngine.h"

namespace Arcane 
{
	struct ApplicationSpecifications
	{
		std::string Name;
		uint32_t WindowWidth;
		uint32_t WindowHeight;
		bool PushImguiLayer = false;
		const char* ProjectFilePath;
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

		AssetDatabase& GetAssetDatabase() { return *m_AssetDatabase; }

		Project* GetProject() { return m_Project; }
	private:
		Window* m_Window;
		static Application* s_Instance;
		std::vector<Layer*> m_LayerStack;

		bool m_bIsRunning = true;
		ImGuiLayer* m_ImGuiLayer;
		CClock* m_Clock;
		bool m_ImguiEnabled = false;

		AssetDatabase* m_AssetDatabase;
		Project* m_Project;
	};
}
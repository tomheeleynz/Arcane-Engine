#include "Application.h"

#include "Arcane/Renderer/Renderer.h"
#include "Arcane/Renderer/Shader.h"
#include "Arcane/Renderer/Pipeline.h"
#include "Arcane/Renderer/RenderPass.h"
#include "Arcane/Scripting/ScriptingEngine.h"
#include "Arcane/Scene/SceneManager.h"
#include "Arcane/Assets/AssetPack.h"

namespace Arcane {
	Application* Application::s_Instance = nullptr;

	Application::Application(ApplicationSpecifications& specifications)
	{
		m_Specs = specifications;

		WindowSpecifications windowSpecifications;
		windowSpecifications.Height = specifications.WindowHeight;
		windowSpecifications.Width = specifications.WindowWidth;
		windowSpecifications.Title = specifications.Name;

		s_Instance = this;

		m_Window = new Window(windowSpecifications);
		m_Window->Init();

		Renderer::Init();

		m_ImguiEnabled = specifications.PushImguiLayer;

		if (m_ImguiEnabled) {
			 m_ImGuiLayer = ImGuiLayer::Create();
			 m_ImGuiLayer->OnAttach();
		}

		m_Clock = new CClock();

		// Init Project
		if (!specifications.RuntimeLayer) {
			ProjectDeserializer deserializer(std::filesystem::path(specifications.ProjectFilePath));
			m_Project = deserializer.Deserialize();
		}

		// Init Scripting Engine
		ScriptingEngine::Init();

		// Init Scene Manager
		SceneManager::Init();

		// Generate Asset Database on application startup
		std::filesystem::path assetPath;

		if (!specifications.RuntimeLayer) {
			m_AssetDatabase = new AssetDatabase(m_Project->GetWorkingPath());
			
			bool assetDatabaseGenerated = m_AssetDatabase->GenerateDatabase();
			if (!assetDatabaseGenerated) {
				std::cout << "Asset Database Not Created\n";
			}
		}
		else {
			m_AssetDatabase = new AssetDatabase();
			AssetPack assetPack(m_AssetDatabase);
			assetPack.Deserialize(specifications.RuntimePath);
		}
	}

	void Application::RenderImGui()
	{
		m_ImGuiLayer->Begin();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::PushLayer(Layer* _layer)
	{
		_layer->OnAttach();
		m_LayerStack.push_back(_layer);
	}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	void Application::Run()
	{
		while (m_bIsRunning)
		{
			m_Clock->Process();
			float deltaTime = m_Clock->GetDeltaTick() / 1000.0f;

			Renderer::BeginFrame();
			{
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(deltaTime);
				}

				if (m_ImguiEnabled) {
					RenderImGui();
					m_ImGuiLayer->End();
				}
			}
			Renderer::EndFrame();

			m_Window->ProcessEvents();
			m_Window->SwapBuffers();
		}
	}
}
#include "Application.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Pipeline.h"
#include "Renderer/RenderPass.h"

Application* Application::s_Instance = nullptr;

Application::Application(ApplicationSpecifications& specifications)
{
	WindowSpecifications windowSpecifications;
	windowSpecifications.Height = specifications.WindowHeight;
	windowSpecifications.Width = specifications.WindowWidth;
	windowSpecifications.Title = specifications.Name;

	s_Instance = this;

	m_Window = new Window(windowSpecifications);
	m_Window->Init();
	
	Renderer::Init();

	m_ImGuiLayer = ImGuiLayer::Create();
	m_ImGuiLayer->OnAttach();
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
		for (Layer* layer : m_LayerStack) {
			layer->OnUpdate(1.0f);
		}

		RenderImGui();
		m_ImGuiLayer->End();

		m_Window->ProcessEvents();
		m_Window->SwapBuffers();
	}
}
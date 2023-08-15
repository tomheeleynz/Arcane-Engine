#include "RuntimeLayer.h"

RuntimeLayer::RuntimeLayer()
{
}

void RuntimeLayer::OnAttach()
{
	// Set up the scene renderer 
	Arcane::SceneRendererSpecs sceneRendererSpecs;
	sceneRendererSpecs.RenderToSwapchain = true;

	m_SceneRenderer = new Arcane::SceneRenderer(sceneRendererSpecs);

	m_ActiveScene = Arcane::SceneManager::GetCurrentScene();
	m_ActiveScene->SetSceneRenderer(m_SceneRenderer);
}

void RuntimeLayer::OnDetach()
{
}

void RuntimeLayer::OnUpdate(float deltaTime)
{
	m_ActiveScene->OnRuntimeUpdate(deltaTime);
}

void RuntimeLayer::OnImGuiRender()
{
}


#pragma once

#include <Arcane.h>

class RuntimeLayer : public Arcane::Layer
{
public:
	RuntimeLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
private:
	Arcane::SceneRenderer* m_SceneRenderer;
	Arcane::Scene* m_ActiveScene;
};
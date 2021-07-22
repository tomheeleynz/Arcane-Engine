#pragma once

#include <Arcane.h>

class EditorLayer : public Arcane::Layer
{
public:
	EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
private:

};
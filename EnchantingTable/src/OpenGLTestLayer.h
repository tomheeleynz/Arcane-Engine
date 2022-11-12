#pragma once
#include <Arcane.h>

class OpenGLTestLayer : public Arcane::Layer
{
public:
	OpenGLTestLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
private:

};
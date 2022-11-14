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
	Arcane::Framebuffer* m_Framebuffer;
	Arcane::RenderPass* m_RenderPass;

};
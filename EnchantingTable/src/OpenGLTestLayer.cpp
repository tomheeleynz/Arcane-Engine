#include <glad/glad.h>
#include "OpenGLTestLayer.h"

OpenGLTestLayer::OpenGLTestLayer()
{
}

void OpenGLTestLayer::OnAttach()
{
	// Shader Setup
	Arcane::Shader* shader;

	// Framebuffer Setup
	Arcane::FramebufferSpecifications specs;
	specs.Width = 800;
	specs.Height = 600;
	m_Framebuffer = Arcane::Framebuffer::Create(specs);

	// Renderpass Setup
	Arcane::RenderPassSpecs renderPassSpecs;
	renderPassSpecs.TargetFramebuffer = m_Framebuffer;
	m_RenderPass = Arcane::RenderPass::Create(renderPassSpecs);

	// Pipeline Setup

}

void OpenGLTestLayer::OnDetach()
{
}

void OpenGLTestLayer::OnUpdate(float deltaTime)
{
	Arcane::Renderer::BeginFrame();
	{
		Arcane::Renderer::BeginRenderPass(m_RenderPass);
		{
			
		}
		Arcane::Renderer::EndRenderPass(m_RenderPass);
	}
	Arcane::Renderer::EndFrame();
}

void OpenGLTestLayer::OnImGuiRender()
{
}

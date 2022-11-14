#include <glad/glad.h>
#include "OpenGLTestLayer.h"

OpenGLTestLayer::OpenGLTestLayer()
{
}

void OpenGLTestLayer::OnAttach()
{
	// Shader
	m_Shader = Arcane::Shader::Create(
		".\\src\\EditorAssets\\Shaders\\TriangleVert.spv",
		".\\src\\EditorAssets\\Shaders\\TriangleFrag.spv"
	);

	// Framebuffer Setup
	Arcane::FramebufferSpecifications specs;
	specs.Width = 800;
	specs.Height = 600;
	specs.ClearColor = {0.2f, 0.3f, 0.3f, 1.0f};
	m_Framebuffer = Arcane::Framebuffer::Create(specs);

	// Renderpass Setup
	Arcane::RenderPassSpecs renderPassSpecs;
	renderPassSpecs.SwapchainFramebuffer = true;
	renderPassSpecs.TargetFramebuffer = m_Framebuffer;
	m_RenderPass = Arcane::RenderPass::Create(renderPassSpecs);

	// Vertex buffer
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	m_VertexBuffer = Arcane::VertexBuffer::Create(vertices, sizeof(float) * 9);

	// Vertex Descriptor
	m_VertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3
	});

	// Pipeline
	Arcane::PipelineSpecification pipelineSpecs;
	pipelineSpecs.descriptor = m_VertexDescriptor;
	pipelineSpecs.shader = m_Shader;
	m_Pipeline = Arcane::Pipeline::Create(pipelineSpecs);
}

void OpenGLTestLayer::OnDetach()
{
}

void OpenGLTestLayer::OnUpdate(float deltaTime)
{
	Arcane::Renderer::BeginRenderPass(m_RenderPass);
	{
		Arcane::Renderer::RenderTriangle(m_VertexBuffer, m_Pipeline);
	}
	Arcane::Renderer::EndRenderPass(m_RenderPass);
}

void OpenGLTestLayer::OnImGuiRender()
{
}

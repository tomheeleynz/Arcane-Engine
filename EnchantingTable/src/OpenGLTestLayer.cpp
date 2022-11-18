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
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	m_VertexBuffer = Arcane::VertexBuffer::Create(vertices, sizeof(float) * 12);
	m_IndexBuffer = Arcane::IndexBuffer::Create(indices, 6);
	m_VertexBuffer->AddIndexBuffer(m_IndexBuffer);

	// Vertex Descriptor
	m_VertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3
	});

	// Pipeline
	Arcane::PipelineSpecification pipelineSpecs;
	pipelineSpecs.descriptor = m_VertexDescriptor;
	pipelineSpecs.shader = m_Shader;
	pipelineSpecs.renderPass = m_RenderPass;
	m_Pipeline = Arcane::Pipeline::Create(pipelineSpecs);
}

void OpenGLTestLayer::OnDetach()
{
}

void OpenGLTestLayer::OnUpdate(float deltaTime)
{
	Arcane::Renderer::BeginRenderPass(m_RenderPass);
	{
		Arcane::Renderer::RenderQuad(m_VertexBuffer, m_Pipeline);
	}
	Arcane::Renderer::EndRenderPass(m_RenderPass);
}

void OpenGLTestLayer::OnImGuiRender()
{
}

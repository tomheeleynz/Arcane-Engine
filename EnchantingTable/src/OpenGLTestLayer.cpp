#include <glad/glad.h>
#include "OpenGLTestLayer.h"

OpenGLTestLayer::OpenGLTestLayer()
{
}

void OpenGLTestLayer::OnAttach()
{
	////////////////////////////////////////////////////////////
	//// Geo Pass
	////////////////////////////////////////////////////////////


	// Generate Texture
	m_Texture = Arcane::Texture::Create(".\\src\\Assets\\Textures\\shield.png");

	// Shader
	m_Shader = Arcane::Shader::Create(
		".\\src\\EditorAssets\\Shaders\\ScreenVert.spv",
		".\\src\\EditorAssets\\Shaders\\ScreenFrag.spv"
	);

	// Framebuffer Setup
	Arcane::FramebufferSpecifications specs;
	
	specs.AttachmentSpecs = {
		Arcane::FramebufferAttachmentType::COLOR
	};
	
	specs.Width = 800;
	specs.Height = 600;
	specs.ClearColor = {0.5f, 0.3f, 0.3f, 1.0f};
	m_Framebuffer = Arcane::Framebuffer::Create(specs);

	// Renderpass Setup
	Arcane::RenderPassSpecs renderPassSpecs;
	renderPassSpecs.SwapchainFramebuffer = false;
	renderPassSpecs.TargetFramebuffer = m_Framebuffer;
	m_RenderPass = Arcane::RenderPass::Create(renderPassSpecs);

	// Vertex buffer
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left 
	};

	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	m_VertexBuffer = Arcane::VertexBuffer::Create(vertices, sizeof(float) * 20);
	m_IndexBuffer = Arcane::IndexBuffer::Create(indices, 6);
	m_VertexBuffer->AddIndexBuffer(m_IndexBuffer);

	// Vertex Descriptor
	m_VertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3,
		Arcane::VertexType::float2
	});

	// Setting up descriptor set 
	Arcane::DescriptorSetSpecs descriptorSetSpecs;
	descriptorSetSpecs.SetNumber = 0;
	m_DescriptorSet = Arcane::DescriptorSet::Create(
		descriptorSetSpecs, {
			{0, 1, Arcane::DescriptorType::SAMPLER, "texSampler", Arcane::DescriptorLocation::FRAGMENT}
		}
	);

	m_DescriptorSet->AddImageSampler(m_Texture, 0, 0);

	// Pipeline
	Arcane::PipelineSpecification pipelineSpecs;
	pipelineSpecs.descriptor = m_VertexDescriptor;
	pipelineSpecs.shader = m_Shader;
	pipelineSpecs.renderPass = m_RenderPass;
	pipelineSpecs.DescriptorSets = { m_DescriptorSet };
	m_Pipeline = Arcane::Pipeline::Create(pipelineSpecs);

	////////////////////////////////////////////////////////////
	//// Composite Pass
	////////////////////////////////////////////////////////////
	m_CompShader = Arcane::Shader::Create(
		".\\src\\EditorAssets\\Shaders\\ScreenVert.spv",
		".\\src\\EditorAssets\\Shaders\\ScreenFrag.spv"
	);

	Arcane::FramebufferSpecifications compSpecs;
	compSpecs.Width = 800;
	compSpecs.Height = 600;
	compSpecs.ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
	m_CompFramebuffer = Arcane::Framebuffer::Create(compSpecs);

	Arcane::RenderPassSpecs compRenderPassSpecs;
	compRenderPassSpecs.SwapchainFramebuffer = true;
	compRenderPassSpecs.TargetFramebuffer = m_CompFramebuffer;
	m_CompRenderPass = Arcane::RenderPass::Create(compRenderPassSpecs);

	m_CompVertexBuffer = Arcane::VertexBuffer::Create(vertices, sizeof(float) * 20);
	m_CompIndexBuffer = Arcane::IndexBuffer::Create(indices, 6);
	m_CompVertexBuffer->AddIndexBuffer(m_CompIndexBuffer);

	Arcane::DescriptorSetSpecs compDescriptorSetSpecs;
	compDescriptorSetSpecs.SetNumber = 0;
	m_CompDescriptorSet = Arcane::DescriptorSet::Create(
		compDescriptorSetSpecs, {
			{ 0, 1, Arcane::DescriptorType::SAMPLER, "texSampler", Arcane::DescriptorLocation::FRAGMENT }
		}
	);

	m_CompDescriptorSet->AddImageSampler(m_Framebuffer, 0, 0);

	Arcane::PipelineSpecification compPipelineSpecs;
	compPipelineSpecs.descriptor = m_VertexDescriptor;
	compPipelineSpecs.shader = m_CompShader;
	compPipelineSpecs.renderPass = m_CompRenderPass;
	compPipelineSpecs.DescriptorSets = { m_CompDescriptorSet };
	m_CompPipeline = Arcane::Pipeline::Create(compPipelineSpecs);
}

void OpenGLTestLayer::OnDetach()
{
}

void OpenGLTestLayer::OnUpdate(float deltaTime)
{
	// Geo Pass
	Arcane::Renderer::BeginRenderPass(m_RenderPass);
	{
		Arcane::Renderer::RenderQuad(m_VertexBuffer, m_Pipeline, { m_DescriptorSet });
	}
	Arcane::Renderer::EndRenderPass(m_RenderPass);

	// Comp RenderPass
	Arcane::Renderer::BeginRenderPass(m_CompRenderPass);
	{
		Arcane::Renderer::RenderQuad(m_CompVertexBuffer, m_CompPipeline, { m_CompDescriptorSet });
	}
	Arcane::Renderer::EndRenderPass(m_CompRenderPass);
}

void OpenGLTestLayer::OnImGuiRender()
{
}

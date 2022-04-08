#include "SceneRenderer.h"

namespace Arcane
{
	struct ScreenVertex
	{
		glm::vec3 position;
		glm::vec2 texture;
	};

	struct SceneRendererData
	{
		// Composite Render Pass
		UniformBuffer* CompositeUniformBuffer;
		TextureSampler* CompositeTextureSampler;
		Framebuffer* CompositeFramebuffer;
		RenderPass* CompositeRenderPass;
		Pipeline* CompositeRenderPipeline;
		VertexBuffer* CompositeVertexBuffer;
		IndexBuffer* CompositeIndexBuffer;
		VertexDescriptor* CompositeVertexDescriptor;
		Shader* CompositeShader;

		// Geomerty Renderpass 
		Framebuffer* GeometryFramebuffer;
		RenderPass* GeometryRenderPass;

		// Meshes to be rendererd
		std::vector<Mesh*> m_Meshes;
	};

	static SceneRendererData s_Data;

	SceneRenderer::SceneRenderer()
	{
		// Setup Geometry Renderpass
		FramebufferSpecifications geometryFramebufferSpecs;
		geometryFramebufferSpecs.AttachmentSpecs = {
			FramebufferAttachmentType::COLOR,
			FramebufferAttachmentType::DEPTH
		};
		
		geometryFramebufferSpecs.ClearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		geometryFramebufferSpecs.Width = 512;
		geometryFramebufferSpecs.Height = 512;
		s_Data.GeometryFramebuffer = Framebuffer::Create(geometryFramebufferSpecs);

		RenderPassSpecs geometryRenderpassSpecs;
		geometryRenderpassSpecs.SwapchainFramebuffer = false;
		geometryRenderpassSpecs.TargetFramebuffer = s_Data.GeometryFramebuffer;
		s_Data.GeometryRenderPass = RenderPass::Create(geometryRenderpassSpecs);

		// Setup Composite Renderpass
		FramebufferSpecifications compositeFramebufferSpecs;
		
		compositeFramebufferSpecs.AttachmentSpecs = {
			FramebufferAttachmentType::COLOR,
			FramebufferAttachmentType::DEPTH
		};

		compositeFramebufferSpecs.ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
		compositeFramebufferSpecs.Width = 512;
		compositeFramebufferSpecs.Height = 512;
		s_Data.CompositeFramebuffer = Framebuffer::Create(compositeFramebufferSpecs);

		RenderPassSpecs compositeSpecs;
		compositeSpecs.SwapchainFramebuffer = false;
		compositeSpecs.TargetFramebuffer = s_Data.CompositeFramebuffer;
		s_Data.CompositeRenderPass = RenderPass::Create(compositeSpecs);

		s_Data.CompositeShader = Shader::Create(
			".\\src\\Assets\\Shaders\\ScreenVert.spv",
			".\\src\\Assets\\Shaders\\ScreenFrag.spv"
		);

		std::vector<ScreenVertex> screenVertices = {
			{{-1.0f,-1.0f,0.0f}, {1.0f, 0.0f}}, // Top Left
			{{ 1.0f,-1.0f,0.0f}, {0.0f, 0.0f}}, // Top Right 
			{{ 1.0f, 1.0f,0.0f}, {0.0f, 1.0f}}, // Bottom Right
			{{-1.0f, 1.0f,0.0f}, {1.0f, 1.0f}}, // Bottom Left
		};

		std::vector<uint32_t> screenIndices = {
			0, 3, 2,
			1, 2, 3
		};

		s_Data.CompositeVertexBuffer = VertexBuffer::Create(screenVertices.data(), screenVertices.size() * sizeof(ScreenVertex));
		s_Data.CompositeIndexBuffer = IndexBuffer::Create(screenIndices.data(), screenIndices.size());
		s_Data.CompositeVertexBuffer->AddIndexBuffer(s_Data.CompositeIndexBuffer);

		s_Data.CompositeVertexDescriptor = VertexDescriptor::Create({
			VertexType::float3,
			VertexType::float2
		});
		
		TextureSampler* CompositeTextureSampler = new TextureSampler(s_Data.GeometryFramebuffer);
		CompositeTextureSampler->SetBinding(0);
		CompositeTextureSampler->SetLocation(UniformDescriptorLocation::FRAGMENT);

		s_Data.CompositeUniformBuffer = UniformBuffer::Create({
			CompositeTextureSampler
		});

		PipelineSpecification compositePipelineSpecs;
		compositePipelineSpecs.shader = s_Data.CompositeShader;
		compositePipelineSpecs.renderPass = s_Data.CompositeRenderPass;
		compositePipelineSpecs.descriptor = s_Data.CompositeVertexDescriptor;
		compositePipelineSpecs.uniformBuffer = s_Data.CompositeUniformBuffer;
		s_Data.CompositeRenderPipeline = Pipeline::Create(compositePipelineSpecs);
	}

	Framebuffer* SceneRenderer::GetFinalRenderFramebuffer()
	{
		return s_Data.CompositeFramebuffer;
	}

	void SceneRenderer::CompositeRenderPass()
	{
		Renderer::BeginRenderPass(s_Data.CompositeRenderPass);
		{
			Renderer::RenderQuad(s_Data.CompositeVertexBuffer, s_Data.CompositeRenderPipeline, s_Data.CompositeUniformBuffer);
		}
		Renderer::EndRenderPass(s_Data.CompositeRenderPass);
	}

	void SceneRenderer::GeometryPass()
	{
		Renderer::BeginRenderPass(s_Data.GeometryRenderPass);
		{	

		}
		Renderer::EndRenderPass(s_Data.GeometryRenderPass);
	}

	void SceneRenderer::RenderScene()
	{
		GeometryPass();
		CompositeRenderPass();
	}

	void SceneRenderer::SubmitMesh(Mesh* mesh) 
	{
		s_Data.m_Meshes.push_back(mesh);
	}
}
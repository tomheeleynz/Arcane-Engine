#include "SceneRenderer.h"

namespace Arcane
{
	struct ScreenVertex
	{
		glm::vec3 position;
		glm::vec2 texture;
	};

	struct MVP {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
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
		Pipeline* GeometryPipeline;
		UniformBuffer* GeometryUniformBuffer;
		UniformObject* MvpObject;
		Shader* GeometryShader;
		VertexDescriptor* GeometryVertexDescriptor;

		// -- Grid (rendered in geometry pipeline)
		Pipeline* GridPipleine;
		Shader* GridShader;
		VertexDescriptor* GridVertexDescriptor;
		VertexBuffer* GridVertexBuffer;
		IndexBuffer* GridIndexBuffer;

		// Meshes to be rendererd
		std::vector<Mesh*> Meshes;

		// Transform for meshes
		std::vector<TransformComponent> MeshTransforms;

		// Camera to render with
		Camera* SceneCamera;
	};

	static SceneRendererData s_Data;

	SceneRenderer::SceneRenderer()
	{
		///////////////////////////////////////////////////////////////
		//// Geometry Renderpass
		///////////////////////////////////////////////////////////////
		FramebufferSpecifications geometryFramebufferSpecs;
		geometryFramebufferSpecs.AttachmentSpecs = {
			FramebufferAttachmentType::COLOR,
			FramebufferAttachmentType::DEPTH
		};
		
		geometryFramebufferSpecs.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		geometryFramebufferSpecs.Width = 512;
		geometryFramebufferSpecs.Height = 512;
		s_Data.GeometryFramebuffer = Framebuffer::Create(geometryFramebufferSpecs);

		RenderPassSpecs geometryRenderpassSpecs;
		geometryRenderpassSpecs.SwapchainFramebuffer = false;
		geometryRenderpassSpecs.TargetFramebuffer = s_Data.GeometryFramebuffer;
		s_Data.GeometryRenderPass = RenderPass::Create(geometryRenderpassSpecs);

		s_Data.GeometryShader = Shader::Create(
			".\\src\\Assets\\Shaders\\MeshVert.spv",
			".\\src\\Assets\\Shaders\\MeshFrag.spv"
		);

		s_Data.GeometryVertexDescriptor = VertexDescriptor::Create({
			VertexType::float3,
			VertexType::float3,
			VertexType::float2
		});

		s_Data.MvpObject = new UniformObject(sizeof(MVP));
		s_Data.MvpObject->SetBinding(0);
		s_Data.MvpObject->SetLocation(UniformDescriptorLocation::VERTEX);

		s_Data.GeometryUniformBuffer = UniformBuffer::Create({
			s_Data.MvpObject
		});

		PipelineSpecification geometrySpecs;
		geometrySpecs.renderPass = s_Data.GeometryRenderPass;
		geometrySpecs.shader = s_Data.GeometryShader;
		geometrySpecs.descriptor = s_Data.GeometryVertexDescriptor;
		geometrySpecs.uniformBuffer = s_Data.GeometryUniformBuffer;
		s_Data.GeometryPipeline = Pipeline::Create(geometrySpecs);

		///////////////////////////////////////////////////////////////
		//// Grid (Part of Geo Pass)
		///////////////////////////////////////////////////////////////
		s_Data.GridShader = Shader::Create(
			".\\src\\Assets\\Shaders\\GridVert.spv",
			".\\src\\Assets\\Shaders\\GridFrag.spv"
		);

		s_Data.GridVertexDescriptor = VertexDescriptor::Create({
			VertexType::float3	
		});

		std::vector<glm::vec3> gridVertices = {
			{-1.0f,-1.0f ,0.0f}, // Top Left
			{ 1.0f,-1.0f, 0.0f}, // Top Right 
			{ 1.0f, 1.0f, 0.0f}, // Bottom Right
			{-1.0f, 1.0f, 0.0f} // Bottom Left
		};

		std::vector<uint32_t> gridIndices = {
			0, 3, 1,
			1, 3, 2
		};

		s_Data.GridVertexBuffer = VertexBuffer::Create(gridVertices.data(), sizeof(glm::vec3) * gridVertices.size());
		s_Data.GridIndexBuffer = IndexBuffer::Create(gridIndices.data(), gridIndices.size());
		s_Data.GridVertexBuffer->AddIndexBuffer(s_Data.GridIndexBuffer);

		PipelineSpecification gridSpecs;
		gridSpecs.renderPass = s_Data.GeometryRenderPass;
		gridSpecs.descriptor = s_Data.GridVertexDescriptor;
		gridSpecs.shader = s_Data.GridShader;
		gridSpecs.uniformBuffer = s_Data.GeometryUniformBuffer;
		s_Data.GridPipleine = Pipeline::Create(gridSpecs);


		///////////////////////////////////////////////////////////////
		//// Composite Renderpass
		///////////////////////////////////////////////////////////////
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
			0, 3, 1,
			1, 3, 2
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
		// Create Uniform Buffer Object
		MVP CurrentMVP;
		CurrentMVP.view = s_Data.SceneCamera->GetView();
		CurrentMVP.proj = s_Data.SceneCamera->GetProject();

		Renderer::BeginRenderPass(s_Data.GeometryRenderPass);
		{	
			for (int i = 0; i < s_Data.Meshes.size(); i++) 
			{
				// Create Transform Component
				TransformComponent& currentMeshComponent = s_Data.MeshTransforms[i];
				glm::mat4 model = glm::translate(glm::mat4(1), currentMeshComponent.pos) * glm::scale(glm::mat4(1), currentMeshComponent.scale);
				CurrentMVP.model = model;
				s_Data.MvpObject->WriteData((void*)&CurrentMVP);

				// Write to uniform object
				s_Data.GeometryUniformBuffer->WriteData(s_Data.MvpObject);

				Renderer::RenderMesh(s_Data.Meshes[i]->GetVertexBuffer(), s_Data.GeometryPipeline, s_Data.GeometryUniformBuffer);
			}
			s_Data.MvpObject->WriteData((void*)&CurrentMVP);
			s_Data.GeometryUniformBuffer->WriteData(s_Data.MvpObject);
			Renderer::RenderQuad(s_Data.GridVertexBuffer, s_Data.GridPipleine, s_Data.GeometryUniformBuffer);
		}
		Renderer::EndRenderPass(s_Data.GeometryRenderPass);
	}

	void SceneRenderer::RenderScene()
	{
		GeometryPass();
		CompositeRenderPass();

		s_Data.Meshes.clear();
		s_Data.MeshTransforms.clear();
	}

	void SceneRenderer::SubmitMesh(Mesh* mesh, TransformComponent& transform) 
	{
		s_Data.Meshes.push_back(mesh);
		s_Data.MeshTransforms.push_back(transform);
	}

	void SceneRenderer::SetCamera(Camera* camera)
	{
		s_Data.SceneCamera = camera;
	}
}
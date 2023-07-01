#include "SceneRenderer.h"

namespace Arcane
{
	struct ScreenVertex
	{
		glm::vec3 position;
		glm::vec2 texture;
	};

	struct CameraData {
		glm::mat4 proj;
		glm::mat4 view;
		glm::vec3 cameraPosition;
	};

	struct Model {
		glm::mat4 transform;
	};

	struct DirectionaLight
	{
		alignas(16) glm::vec3 direction;
		alignas(16) glm::vec3 color;
	};

	struct SceneRendererData
	{
		// Global Render Data
		DescriptorSet* GlobalDescriptorSet;
		UniformBuffer* GlobalUniformBuffer;

		// Composite Render Pass
		DescriptorSet* CompositeDescriptorSet;
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
		DescriptorSet* GeometryPassDescriptorSet;
		DescriptorSet* ObjectDescriptorSet;
		UniformBuffer* ObjectUniformBuffer;
		
		// Test for new rendering
		std::vector<DescriptorSet*> ObjectSets;
		std::vector<UniformBuffer*> ObjectUniformBuffers;
		
		UniformBuffer* GeometryPassUniformBuffer;
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

		// Materials for meshses
		std::vector<Material*> materials;

		// Camera to render with
		Camera* SceneCamera;

		// Scene Lighting Stuff
		LightComponent directionaLight;
		TransformComponent directionalLightTransform;
	};

	static SceneRendererData s_Data;

	SceneRenderer::SceneRenderer()
	{
		///////////////////////////////////////////////////////////////
		//// Global Render Data
		///////////////////////////////////////////////////////////////
		printf("----------- Creating Global Data\n");
		// -- Create Descripor Set
		DescriptorSetSpecs globalDescriptorSetSpecs;
		globalDescriptorSetSpecs.SetNumber = 0;
		s_Data.GlobalDescriptorSet = DescriptorSet::Create(
			globalDescriptorSetSpecs, {
				{0, 1, DescriptorType::UNIFORM_BUFFER, "Camera Data", DescriptorLocation::VERTEX}
			}
		);

		// -- Create Uniform Buffer, then add to descriptor set
		s_Data.GlobalUniformBuffer = UniformBuffer::Create(sizeof(CameraData));
		s_Data.GlobalDescriptorSet->AddUniformBuffer(
			s_Data.GlobalUniformBuffer, 0, 0
		);


		///////////////////////////////////////////////////////////////
		//// Geometry Renderpass
		///////////////////////////////////////////////////////////////
		printf("----------- Creating Geo Renderpass\n");
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

		DescriptorSetSpecs geometryPassDescriptorSetSpecs;
		geometryPassDescriptorSetSpecs.SetNumber = 1;
		s_Data.GeometryPassDescriptorSet = DescriptorSet::Create(
			geometryPassDescriptorSetSpecs, {
				{0, 1, DescriptorType::UNIFORM_BUFFER, "Lights", DescriptorLocation::FRAGMENT}
			}
		);

		s_Data.GeometryPassUniformBuffer = UniformBuffer::Create(sizeof(DirectionaLight));
		s_Data.GeometryPassDescriptorSet->AddUniformBuffer(s_Data.GeometryPassUniformBuffer, 1, 0);

		///////////////////////////////////////////////////////////////
		//// Grid (Part of Geo Pass)
		///////////////////////////////////////////////////////////////
		s_Data.GridShader = ShaderLibrary::GetShader("Grid");

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

		// Create Descriptor Sets
		DescriptorSetSpecs objectSetSpecs;
		objectSetSpecs.SetNumber = 3;
		s_Data.ObjectDescriptorSet = DescriptorSet::Create(objectSetSpecs, {
			{0, 1, DescriptorType::UNIFORM_BUFFER, "Transform Data", DescriptorLocation::VERTEX}
		});
		s_Data.ObjectUniformBuffer = UniformBuffer::Create(sizeof(Model));
		s_Data.ObjectDescriptorSet->AddUniformBuffer(s_Data.ObjectUniformBuffer, 3, 0);

		s_Data.ObjectSets.resize(5);
		s_Data.ObjectUniformBuffers.resize(5);

		for (int i = 0; i < 5; i++) {
			DescriptorSetSpecs newObjectSetSpecs;
			newObjectSetSpecs.SetNumber = 3;

			s_Data.ObjectSets[i] = DescriptorSet::Create(newObjectSetSpecs, {
				{0, 1, DescriptorType::UNIFORM_BUFFER, "Transform Data", DescriptorLocation::VERTEX}
			});

			s_Data.ObjectUniformBuffers[i] = UniformBuffer::Create(sizeof(Model));
			s_Data.ObjectSets[i]->AddUniformBuffer(s_Data.ObjectUniformBuffers[i], 3, 0);
		}


		PipelineSpecification gridSpecs;
		gridSpecs.renderPass = s_Data.GeometryRenderPass;
		gridSpecs.descriptor = s_Data.GridVertexDescriptor;
		gridSpecs.shader = s_Data.GridShader;
		gridSpecs.DescriptorSets = {s_Data.GlobalDescriptorSet};
		s_Data.GridPipleine = Pipeline::Create(gridSpecs);


		///////////////////////////////////////////////////////////////
		//// Composite Renderpass
		///////////////////////////////////////////////////////////////
		printf("----------- Creating Composite Renderpass\n");
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

		s_Data.CompositeShader = ShaderLibrary::GetShader("Screen");

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

		// Create per pass Descriptor
		DescriptorSetSpecs compositeSetSpecs;
		compositeSetSpecs.SetNumber = 1;
		s_Data.CompositeDescriptorSet = DescriptorSet::Create(compositeSetSpecs, {
			{0, 1, DescriptorType::SAMPLER, "Geo Framebuffer Texture", DescriptorLocation::FRAGMENT}
		});

		s_Data.CompositeDescriptorSet->AddImageSampler(s_Data.GeometryFramebuffer, 1, 0);

		PipelineSpecification compositePipelineSpecs;
		compositePipelineSpecs.shader = s_Data.CompositeShader;
		compositePipelineSpecs.renderPass = s_Data.CompositeRenderPass;
		compositePipelineSpecs.descriptor = s_Data.CompositeVertexDescriptor;
		compositePipelineSpecs.DescriptorSets = { s_Data.CompositeDescriptorSet };
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
			Renderer::RenderQuad(s_Data.CompositeVertexBuffer, s_Data.CompositeRenderPipeline, {s_Data.CompositeDescriptorSet});
		}
		Renderer::EndRenderPass(s_Data.CompositeRenderPass);
	}

	void SceneRenderer::GeometryPass()
	{
		DirectionaLight currentDirLight;
		currentDirLight.direction = s_Data.directionalLightTransform.pos;
		currentDirLight.color = s_Data.directionaLight.color;
		s_Data.GeometryPassUniformBuffer->WriteData((void*)&currentDirLight, sizeof(DirectionaLight));

		// Update any per pass resources
		Renderer::BeginRenderPass(s_Data.GeometryRenderPass);
		{
			for (int i = 0; i < s_Data.Meshes.size(); i++)
			{
				Mesh* currentMesh = s_Data.Meshes[i];
				Material* material = s_Data.materials[i];

				// Create Transform Component
				TransformComponent& currentMeshComponent = s_Data.MeshTransforms[i];

				// Create Model Matrix
				Model currentTransform;
				currentTransform.transform = glm::translate(glm::mat4(1), currentMeshComponent.pos) *
					glm::mat4(glm::quat(currentMeshComponent.rotation)) *
					glm::scale(glm::mat4(1), currentMeshComponent.scale);

				// Write to uniform buffer
				s_Data.ObjectUniformBuffers[i]->WriteData((void*)&currentTransform, sizeof(Model));

				for (int j = 0; j < currentMesh->GetSubMeshes().size(); j++) {
					SubMesh* currentSubMesh = currentMesh->GetSubMeshes()[j];
					Renderer::RenderMesh(currentSubMesh->GetVertexBuffer(), material->GetPipeline(), {
						s_Data.GlobalDescriptorSet,
						s_Data.GeometryPassDescriptorSet,
						material->GetDescriptorSet(),
						s_Data.ObjectSets[i]
					});
				}
			}

			if (m_RenderGrid)
				Renderer::RenderQuad(s_Data.GridVertexBuffer, s_Data.GridPipleine, { s_Data.GlobalDescriptorSet });
		}
		Renderer::EndRenderPass(s_Data.GeometryRenderPass);
	}

	void SceneRenderer::RenderScene()
	{
		// Write to uniform buffer
		CameraData currentFrameCameraData;
		currentFrameCameraData.proj = s_Data.SceneCamera->GetProject();
		
		// Need to to this for vulkan coord system
		currentFrameCameraData.proj[1][1] *= -1;
		currentFrameCameraData.view = s_Data.SceneCamera->GetView();
		currentFrameCameraData.cameraPosition = s_Data.SceneCamera->GetPosition();
		s_Data.GlobalUniformBuffer->WriteData((void*)&currentFrameCameraData, sizeof(CameraData));

		GeometryPass();
		CompositeRenderPass();

		s_Data.Meshes.clear();
		s_Data.MeshTransforms.clear();
		s_Data.materials.clear();
	}

	void SceneRenderer::SubmitMesh(Mesh* mesh, TransformComponent& transform, Material* material) 
	{
		s_Data.Meshes.push_back(mesh);
		s_Data.MeshTransforms.push_back(transform);

		// Set Renderpass to material
		material->SetRenderPass(s_Data.GeometryRenderPass);
		material->SetGlobalData(s_Data.GlobalDescriptorSet);
		material->SetFrameData(s_Data.GeometryPassDescriptorSet);
		material->SetDrawData(s_Data.ObjectDescriptorSet);
		
		s_Data.materials.push_back(material);
	}

	void SceneRenderer::SetCamera(Camera* camera)
	{
		s_Data.SceneCamera = camera;
	}

	void SceneRenderer::ResizeScene(uint32_t width, uint32_t height) {
		m_SceneSize = { width, height };

		// Update Geo Framebuffer
		s_Data.GeometryFramebuffer->Resize(width, height);
		s_Data.CompositeDescriptorSet->AddImageSampler(s_Data.GeometryFramebuffer, 1, 0);
		s_Data.CompositeFramebuffer->Resize(width, height);
	}

	void SceneRenderer::SetDirectionalLight(LightComponent& light, TransformComponent& transform)
	{
		s_Data.directionaLight = light;
		s_Data.directionalLightTransform = transform;
	}
}
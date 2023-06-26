#include "SceneRenderer2D.h"
#include "Renderer.h"

namespace Arcane
{
	struct RenderQuad
	{
		Quad* quad;
		glm::vec3 color;
		Material* material;
		TransformComponent transform;
	};

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
	
	struct SceneRenderer2DData
	{
		// Global uniforms
		DescriptorSet* GlobalDescriptorSet;
		UniformBuffer* GlobalUniformBuffer;

		// Composite Renderpass
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

		std::vector<DescriptorSet*> ObjectSets;
		std::vector<UniformBuffer*> ObjectUniformBuffers;

		UniformBuffer* GeometryPassUniformBuffer;
		Shader* GeometryShader;
		VertexDescriptor* GeometryVertexDescriptor;

		std::vector<RenderQuad> Quads;

		// Camera to render with
		Camera* SceneCamera;
	};

	static SceneRenderer2DData s_Data;

	SceneRenderer2D::SceneRenderer2D()
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

		geometryFramebufferSpecs.ClearColor = { 0.149f, 0.301f, 0.556f, 1.0f };
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
			geometryPassDescriptorSetSpecs, {}
		);

		DescriptorSetSpecs objectSetSpecs;
		objectSetSpecs.SetNumber = 3;
		s_Data.ObjectDescriptorSet = DescriptorSet::Create(objectSetSpecs, {
			{0, 1, DescriptorType::UNIFORM_BUFFER, "Transform Data", DescriptorLocation::VERTEX} });
		
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

	void SceneRenderer2D::RenderScene()
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

		s_Data.Quads.clear();
	}

	void SceneRenderer2D::SetCamera(Camera* camera)
	{
		s_Data.SceneCamera = camera;
	}

	void SceneRenderer2D::ResizeScene(uint32_t width, uint32_t height)
	{
		m_SceneSize = { width, height };

		// Update Geo Framebuffer
		s_Data.GeometryFramebuffer->Resize(width, height);
		s_Data.CompositeDescriptorSet->AddImageSampler(s_Data.GeometryFramebuffer, 1, 0);
		s_Data.CompositeFramebuffer->Resize(width, height);
	}

	void SceneRenderer2D::SubmitQuad(Quad* quad, TransformComponent& component, glm::vec3 color, Material* material)
	{
		RenderQuad newRenderQuad;
		newRenderQuad.color = color;
		newRenderQuad.material = material;
		newRenderQuad.transform = component;
		newRenderQuad.quad = quad;

		material->SetRenderPass(s_Data.GeometryRenderPass);
		material->SetGlobalData(s_Data.GlobalDescriptorSet);
		material->SetFrameData(s_Data.GeometryPassDescriptorSet);
		material->SetDrawData(s_Data.ObjectDescriptorSet);

		s_Data.Quads.push_back(newRenderQuad);
	}

	Framebuffer* SceneRenderer2D::GetFinalRenderFramebuffer()
	{
		return s_Data.CompositeFramebuffer;
	}

	void SceneRenderer2D::CompositeRenderPass()
	{
		Renderer::BeginRenderPass(s_Data.CompositeRenderPass);
		{
			Renderer::RenderQuad(s_Data.CompositeVertexBuffer, s_Data.CompositeRenderPipeline, { s_Data.CompositeDescriptorSet });
		}
		Renderer::EndRenderPass(s_Data.CompositeRenderPass);
	}

	void SceneRenderer2D::GeometryPass()
	{
		Renderer::BeginRenderPass(s_Data.GeometryRenderPass);
		{
			for (int i = 0; i < s_Data.Quads.size(); i++)  {
				RenderQuad renderQuad = s_Data.Quads[i];

				Model currentTransform;
				currentTransform.transform = glm::translate(glm::mat4(1), renderQuad.transform.pos) *
					glm::mat4(glm::quat(renderQuad.transform.rotation)) *
					glm::scale(glm::mat4(1), renderQuad.transform.scale);

				s_Data.ObjectUniformBuffers[i]->WriteData((void*)&currentTransform, sizeof(Model));

				Renderer::RenderQuad(renderQuad.quad->GetVertexBuffer(), renderQuad.material->GetPipeline(), {
					s_Data.GlobalDescriptorSet,
					s_Data.GeometryPassDescriptorSet,
					renderQuad.material->GetDescriptorSet(),
					s_Data.ObjectSets[i]
				});
			}
		}
		Renderer::EndRenderPass(s_Data.GeometryRenderPass);
	}
}
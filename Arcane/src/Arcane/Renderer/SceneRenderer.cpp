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

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
		int texId;
	};

	struct AnimatedQuadVertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
	};

	struct AnimData
	{
		float currentFrameCountX;
		float currentFrameCountY;
		float totalFrameCountX;
		float totalFrameCountY;
	};

	enum class BatchTextureType
	{
		BASE,
		ASSET
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

		////////////////////////////////////////////////////////////////////////////////
		//// 2D Batch Rendering info
		////////////////////////////////////////////////////////////////////////////////
		int maxQuads = 100;
		int maxQuadVertices = maxQuads * 4;
		int quadCount = 0;
		int quadIndicesCount = 0;
		VertexDescriptor* QuadVertexDescriptor;
		VertexBuffer* QuadVertexBuffer;
		IndexBuffer* QuadIndexBuffer;
		uint32_t* quadIndices;
		glm::vec4 quadBase[4];
		QuadVertex* quadVertices;
		Shader* SpriteShader;
		Pipeline* QuadPipeline;
		DescriptorSet* QuadDescriptorSet;
		Texture* QuadBaseTexture;

		std::vector<Texture*> SpriteTextures;
		std::vector<BatchTextureType> SpriteTextureTypes;

		////////////////////////////////////////////////////////////////////////////////
		//// 2D Animated Rendering Info
		////////////////////////////////////////////////////////////////////////////////
		std::vector<VertexBuffer*> AnimatedQuadBuffers;
		std::vector<IndexBuffer*> AnimatedQuadIndexBuffers;
		std::vector<Texture*> AnimatedQuadTextures;
		std::vector<AnimData> AnimatedQuadAnimationData;

		Shader* SpriteAnimatedShader;
		Pipeline* SpriteAnimatedPipeline;
		DescriptorSet* SpriteAnimatedDescriptorSet;
		UniformBuffer* AnimDataUniformBuffer;
		VertexDescriptor* SpriteAnimatedVertexDescriptor;
		
		
		int animatedQuadCount = 0;
		int maxAnimatedQuads = 10;
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
			FramebufferAttachmentType::R32_INT,
			FramebufferAttachmentType::DEPTH
		};

		geometryFramebufferSpecs.ClearColor = { 0.19215686274509805f, 0.30196078431372547f, 0.4745098039215686f, 1.0f };
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
		gridSpecs.topolgy = PrimativeTopology::TRIANGLE_STRIP;
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
		compositeSetSpecs.SetNumber = 0;
		s_Data.CompositeDescriptorSet = DescriptorSet::Create(compositeSetSpecs, {
			{0, 1, DescriptorType::SAMPLER, "Geo Framebuffer Texture", DescriptorLocation::FRAGMENT}
		});

		s_Data.CompositeDescriptorSet->AddImageSampler(s_Data.GeometryFramebuffer, 1, 0);

		PipelineSpecification compositePipelineSpecs;
		compositePipelineSpecs.shader = s_Data.CompositeShader;
		compositePipelineSpecs.renderPass = s_Data.CompositeRenderPass;
		compositePipelineSpecs.descriptor = s_Data.CompositeVertexDescriptor;
		compositePipelineSpecs.topolgy = PrimativeTopology::TRIANGLE_STRIP;
		compositePipelineSpecs.DescriptorSets = { s_Data.CompositeDescriptorSet };
		s_Data.CompositeRenderPipeline = Pipeline::Create(compositePipelineSpecs);


		////////////////////////////////////////////////////////////////////////////////////
		//// 2D Batch Rendering
		////////////////////////////////////////////////////////////////////////////////////
		s_Data.quadIndices = new uint32_t[s_Data.maxQuads * 6];
		
		s_Data.quadBase[0] = { -0.5f,-0.5f, 0.0f, 1.0f };
		s_Data.quadBase[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadBase[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.quadBase[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.quadVertices = new QuadVertex[s_Data.maxQuadVertices];

		int offset = 0;
		for (int index = 0; index < s_Data.maxQuads * 6; index += 6) {
			s_Data.quadIndices[index] = (offset * 4) + 0;
			s_Data.quadIndices[index + 1] = (offset * 4) + 1;
			s_Data.quadIndices[index + 2] = (offset * 4) + 3;
			s_Data.quadIndices[index + 3] = (offset * 4) + 1;
			s_Data.quadIndices[index + 4] = (offset * 4) + 2;
			s_Data.quadIndices[index + 5] = (offset * 4) + 3;
			offset++;
		}

		s_Data.QuadVertexDescriptor = VertexDescriptor::Create({
			VertexType::float3,
			VertexType::float3,
			VertexType::float2,
			VertexType::integer
		});

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.quadVertices, s_Data.maxQuadVertices * sizeof(QuadVertex));
		s_Data.QuadIndexBuffer = IndexBuffer::Create(s_Data.quadIndices, s_Data.maxQuads * 6);
		s_Data.QuadVertexBuffer->AddIndexBuffer(s_Data.QuadIndexBuffer);

		s_Data.SpriteShader = ShaderLibrary::GetShader("Sprite-Default");
		s_Data.QuadBaseTexture = Texture::Create(255.0f, 255.0f, 255.0f, 255.0f);

		s_Data.SpriteTextures.resize(32);
		s_Data.SpriteTextureTypes.resize(32);

		for (int i = 0; i < s_Data.SpriteTextures.size(); i++)
		{
			s_Data.SpriteTextures[i] = s_Data.QuadBaseTexture;
			s_Data.SpriteTextureTypes[i] = BatchTextureType::BASE;
		}

		DescriptorSetSpecs quadSetSpecs;
		quadSetSpecs.SetNumber = 1;
		s_Data.QuadDescriptorSet = DescriptorSet::Create(quadSetSpecs, {
			{0, 32, DescriptorType::SAMPLER, "sprites", DescriptorLocation::FRAGMENT}
		});

		s_Data.QuadDescriptorSet->AddImageSampler(s_Data.QuadBaseTexture, 1, 0);

		PipelineSpecification quadPipelineSpecs;
		quadPipelineSpecs.shader = s_Data.SpriteShader;
		quadPipelineSpecs.renderPass = s_Data.GeometryRenderPass;
		quadPipelineSpecs.descriptor = s_Data.QuadVertexDescriptor;
		quadPipelineSpecs.DescriptorSets = { s_Data.GlobalDescriptorSet, s_Data.QuadDescriptorSet };
		quadPipelineSpecs.topolgy = PrimativeTopology::TRIANGLE_STRIP;
		s_Data.QuadPipeline = Pipeline::Create(quadPipelineSpecs);


		////////////////////////////////////////////////////////////////////////////////////
		//// 2D Animation Render (needs to be seperate buffers (probably?))
		////////////////////////////////////////////////////////////////////////////////////
		s_Data.AnimatedQuadBuffers.resize(s_Data.maxAnimatedQuads);
		s_Data.AnimatedQuadIndexBuffers.resize(s_Data.maxAnimatedQuads);
		s_Data.AnimatedQuadTextures.resize(s_Data.maxAnimatedQuads);
		s_Data.AnimatedQuadAnimationData.resize(s_Data.maxAnimatedQuads);

		std::vector<uint32_t> animatedQuadIndices = {
			0, 1, 3,
			1, 2, 3
		};

		for (int i = 0; i < s_Data.maxAnimatedQuads; i++) {
			s_Data.AnimatedQuadBuffers[i] = VertexBuffer::Create(sizeof(AnimatedQuadVertex) * 4);
			s_Data.AnimatedQuadIndexBuffers[i] = IndexBuffer::Create(animatedQuadIndices.data(), 6);
			s_Data.AnimatedQuadBuffers[i]->AddIndexBuffer(s_Data.AnimatedQuadIndexBuffers[i]);
		}

		s_Data.SpriteAnimatedVertexDescriptor = VertexDescriptor::Create({
			VertexType::float3,
			VertexType::float3,
			VertexType::float2
		});

		s_Data.SpriteAnimatedShader = ShaderLibrary::GetShader("Sprite-Animated");

		DescriptorSetSpecs spriteAnimatedDescriptorSpecs;
		spriteAnimatedDescriptorSpecs.SetNumber = 1;

		s_Data.SpriteAnimatedDescriptorSet = DescriptorSet::Create(spriteAnimatedDescriptorSpecs, {
			{0, 1, DescriptorType::SAMPLER, "sprite", DescriptorLocation::FRAGMENT},
			{1, 1, DescriptorType::UNIFORM_BUFFER, "animData", DescriptorLocation::FRAGMENT}
		});

		s_Data.AnimDataUniformBuffer = UniformBuffer::Create(sizeof(AnimData));
		s_Data.SpriteAnimatedDescriptorSet->AddUniformBuffer(s_Data.AnimDataUniformBuffer, 1, 1);

		PipelineSpecification animatedQuadPipelineSpecs;
		animatedQuadPipelineSpecs.shader = s_Data.SpriteAnimatedShader;
		animatedQuadPipelineSpecs.renderPass = s_Data.GeometryRenderPass;
		animatedQuadPipelineSpecs.descriptor = s_Data.SpriteAnimatedVertexDescriptor;
		animatedQuadPipelineSpecs.DescriptorSets = { s_Data.GlobalDescriptorSet, s_Data.SpriteAnimatedDescriptorSet };
		animatedQuadPipelineSpecs.topolgy = PrimativeTopology::TRIANGLE_STRIP;
		s_Data.SpriteAnimatedPipeline = Pipeline::Create(animatedQuadPipelineSpecs);
	}

	Framebuffer* SceneRenderer::GetFinalRenderFramebuffer()
	{
		return s_Data.CompositeFramebuffer;
	}

	void SceneRenderer::ReadGeometryFramebufferPixel(uint32_t index)
	{
		int test = s_Data.GeometryFramebuffer->ReadPixel(1);
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
		
		// 3D Geometry Pass
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
			/*if (m_RenderGrid)
				Renderer::RenderQuad(s_Data.GridVertexBuffer, s_Data.GridPipleine, { s_Data.GlobalDescriptorSet });*/
		}

		// 2D Renderer
		{
			// Batch non animated quads
			if (s_Data.quadCount > 0) {
				s_Data.QuadDescriptorSet->AddImageSamplerArray(s_Data.SpriteTextures, 1, 0);
				s_Data.QuadVertexBuffer->SetData(s_Data.quadVertices, s_Data.quadCount * 4 * sizeof(QuadVertex));
				Renderer::RenderQuad(s_Data.QuadVertexBuffer, s_Data.QuadPipeline, {s_Data.GlobalDescriptorSet, s_Data.QuadDescriptorSet});
			}

			// Render Animated Quads Seperatly
			if (s_Data.animatedQuadCount > 0)
			{
				for (int i = 0; i < s_Data.animatedQuadCount; i++)
				{
					// Set Uniforms
					AnimData currentAnimData = s_Data.AnimatedQuadAnimationData[i];
					Texture* currentTexture = s_Data.AnimatedQuadTextures[i];

					s_Data.AnimDataUniformBuffer->WriteData((void*)&currentAnimData, sizeof(AnimData));
					s_Data.SpriteAnimatedDescriptorSet->AddImageSampler(currentTexture, 1, 0);

					// Render Quad
					Renderer::RenderQuad(s_Data.AnimatedQuadBuffers[i], s_Data.SpriteAnimatedPipeline, {
						s_Data.GlobalDescriptorSet,
						s_Data.SpriteAnimatedDescriptorSet
					});
				}
			}
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

		///////////////////////////////
		/// 2D Batch Cleanup
		///////////////////////////////
		delete s_Data.quadVertices;
		s_Data.quadVertices = new QuadVertex[s_Data.maxQuadVertices];
		s_Data.quadCount = 0;
		s_Data.animatedQuadCount = 0;
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

	void SceneRenderer::SubmitQuad(TransformComponent& transformComponent, SpriteRendererComponent& spriteRendererComponent)
	{
		int quadSize = 4;

		glm::mat4 translation = glm::translate(glm::mat4(1.0), transformComponent.pos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), transformComponent.scale);
		glm::mat4 rotation = glm::mat4(glm::quat(transformComponent.rotation));

		glm::vec2 textureCoords[4] = {
			{1.0f, 0.0f}, 
			{0.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 1.0f}
		};

		int textureId = -1;
		int lastBaseIndex = 0;
		
		for (int i = 1; i < s_Data.SpriteTextures.size(); i++) {
			if (spriteRendererComponent.sprite == nullptr) {
				textureId = 0;
				break;
			}
			else {
				if (spriteRendererComponent.sprite->GetID() == s_Data.SpriteTextures[i]->GetID() && s_Data.SpriteTextureTypes[i] == BatchTextureType::ASSET)
				{
					textureId = i;
					break;
				}
				else if (s_Data.SpriteTextureTypes[i] == BatchTextureType::BASE) {
					lastBaseIndex = i;
					break;
				}
			}
		}

		if (textureId == -1)
		{
			s_Data.SpriteTextures[lastBaseIndex] = spriteRendererComponent.sprite;
			s_Data.SpriteTextureTypes[lastBaseIndex] = BatchTextureType::ASSET;
			textureId = lastBaseIndex;
		}


		for (int i = 0; i < quadSize; i++) {
			QuadVertex v;
			v.position = translation * rotation * scale * s_Data.quadBase[i];
			v.color = spriteRendererComponent.color;
			v.texCoord = textureCoords[i];
			v.texId = textureId;
			s_Data.quadVertices[(s_Data.quadCount * 4) + i] = v;
		}

		s_Data.quadCount++;
	}

	void SceneRenderer::SubmitAnimatedQuad(TransformComponent& transformComponent, SpriteRendererComponent& spriteRendererComponent, Animator& animatorComponent)
	{
		int quadSize = 4;

		glm::mat4 translation = glm::translate(glm::mat4(1.0), transformComponent.pos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), transformComponent.scale);
		glm::mat4 rotation = glm::mat4(glm::quat(transformComponent.rotation));

		glm::vec2 textureCoords[4] = {
			{1.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 1.0f}
		};

		std::vector<AnimatedQuadVertex> vertices;
		
		for (int i = 0; i < quadSize; i++) {
			AnimatedQuadVertex v;
			v.position = translation * rotation * scale * s_Data.quadBase[i];
			v.color = spriteRendererComponent.color;
			v.texCoord = textureCoords[i];
			vertices.push_back(v);
		}

		AnimData newData;

		Animation* currentAnimation = animatorComponent.controller->GetCurrentAnimation();
		KeyFrame* currentKeyFrame = currentAnimation->GetCurrentKeyFrame();

		if (currentKeyFrame->GetType() == KeyFrameType::TWO_DIMENSIONAL)
		{
			KeyFrame2D* currentKeyFrame2D = static_cast<KeyFrame2D*>(currentKeyFrame);
			
			if (currentAnimation->CurrentFrameCount >= currentKeyFrame2D->GetKeyFrameLength()) {
				currentAnimation->SetNextKeyFrame();
				currentAnimation->CurrentFrameCount = 0;
			}
			else {
				currentAnimation->CurrentFrameCount += 1;
			}

			newData.currentFrameCountX = currentKeyFrame2D->GetImageIndexX();
			newData.currentFrameCountY = currentKeyFrame2D->GetImageIndexY();
		}

		newData.totalFrameCountX = 6;
		newData.totalFrameCountY = 1;

		if (spriteRendererComponent.sprite == nullptr)
			s_Data.AnimatedQuadTextures[s_Data.animatedQuadCount] = s_Data.QuadBaseTexture;
		else
			s_Data.AnimatedQuadTextures[s_Data.animatedQuadCount] = spriteRendererComponent.sprite;

		s_Data.AnimatedQuadAnimationData[s_Data.animatedQuadCount] = newData;
		s_Data.AnimatedQuadBuffers[s_Data.animatedQuadCount]->SetData(vertices.data(), vertices.size() * sizeof(AnimatedQuadVertex));
		s_Data.animatedQuadCount++;
	}

	void SceneRenderer::SetCamera(Camera* camera)
	{
		s_Data.SceneCamera = camera;
	}

	void SceneRenderer::ResizeScene(uint32_t width, uint32_t height) {
		m_SceneSize = { width, height };

		// Update Geo Framebuffer
		s_Data.GeometryFramebuffer->Resize(width, height);
		s_Data.CompositeDescriptorSet->AddImageSampler(s_Data.GeometryFramebuffer, 1,  0);
		s_Data.CompositeFramebuffer->Resize(width, height);
	}

	void SceneRenderer::SetDirectionalLight(LightComponent& light, TransformComponent& transform)
	{
		s_Data.directionaLight = light;
		s_Data.directionalLightTransform = transform;
	}
}
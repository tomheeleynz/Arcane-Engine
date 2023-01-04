#pragma once
#include <vector>

#include "Buffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "UniformBuffer.h"
#include "DescriptorSet.h"
#include "Material.h"

namespace Arcane {

	enum class RendererAPIType
	{
		None = 0,
		Vulkan,
		OpenGL
	};

	class RendererAPI
	{
	public:
		// Renderer API Functions
		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void BeginRenderPass(RenderPass* renderPass) = 0;
		virtual void EndRenderPass(RenderPass* renderPass) = 0;

		// Render Functions
		virtual void RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline) = 0;

		// -- Without Uniform Buffer
		virtual void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline) = 0;
		virtual void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets) = 0;

		// Mesh
		virtual void RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets) = 0;
		virtual void RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* materal) = 0;

		// Static Renderer Functions
		static void SetAPI(RendererAPIType api);
		static RendererAPIType Current() { return s_CurrentRendererAPI; }
	private:
		inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::Vulkan;
	};

}
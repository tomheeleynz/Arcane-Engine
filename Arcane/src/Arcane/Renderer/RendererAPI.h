#pragma once

#include "Buffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "UniformBuffer.h"
#include "DescriptorSet.h"

namespace Arcane {

	enum class RendererAPIType
	{
		None = 0,
		Vulkan
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
		virtual void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, DescriptorSet* descriptorSet) = 0;

		// Mesh
		virtual void RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, DescriptorSet* descriptorSet) = 0;

		// Static Renderer Functions
		static void SetAPI(RendererAPIType api);
		static RendererAPIType Current() { return s_CurrentRendererAPI; }
	private:
		inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::Vulkan;
	};

}
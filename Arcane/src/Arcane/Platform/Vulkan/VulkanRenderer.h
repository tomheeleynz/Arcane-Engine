#pragma once

#include <vulkan/vulkan.h>
#include "Arcane/Renderer/RendererAPI.h"

namespace Arcane {
	class VulkanRenderer : public RendererAPI
	{
	public:
		VulkanRenderer();

		void Init();
		void Shutdown() override;
		
		void BeginFrame() override;
		void EndFrame() override;

		void BeginRenderPass(RenderPass* renderPass) override;
		void EndRenderPass(RenderPass* renderPass) override;

		// Render Functions
		void RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline) override;

		// Render Quad Functions
		void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline) override;
		void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets);

		void RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets) override;
		void RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* material) override;
	private:

	};
}
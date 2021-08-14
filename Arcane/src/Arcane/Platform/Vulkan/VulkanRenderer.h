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
		void BeginRenderPass(RenderPass* renderPass) override;
		void EndRenderPass(RenderPass* renderPass) override;

		// Render Functions
		void RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline) override;


		// Render Quad Functions
		// -- Without Uniform Buffer
		void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline) override;

	private:

	};
}
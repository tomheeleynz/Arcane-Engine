#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/RendererAPI.h"

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
	void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline) override;
private:

};
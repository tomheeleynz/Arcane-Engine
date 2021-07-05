#pragma once

#include "Buffer.h"
#include "Pipeline.h"
#include "RendererAPI.h"
#include "RenderPass.h"

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	// Begin and End Functions
	static void BeginRenderPass(RenderPass* renderPass);
	static void EndRenderPass(RenderPass* renderPass);

	// Render Functions
	static void RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline);
	static void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline);
private:
};
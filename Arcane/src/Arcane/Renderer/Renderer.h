#pragma once

#include <vector>

#include "Buffer.h"
#include "Pipeline.h"
#include "RendererAPI.h"
#include "RenderPass.h"
#include "Material.h"

namespace Arcane {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		// Begin and End Functions
		static void BeginFrame();
		static void EndFrame();
		static void BeginRenderPass(RenderPass* renderPass);
		static void EndRenderPass(RenderPass* renderPass);

		// Render Functions
		static void RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline);
		static void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline);
		static void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets);

		// Render Mesh
		static void RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets);
		static void RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* material);
	private:
	};
}
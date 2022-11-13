#pragma once
#include "Arcane/Renderer/RendererAPI.h"

namespace Arcane
{
	class OpenGLRenderer : public RendererAPI
	{
	public:
		OpenGLRenderer();

		void Shutdown() override;

		void BeginFrame() override;
		void EndFrame() override;

		void BeginRenderPass(RenderPass* renderPass) override;
		void EndRenderPass(RenderPass* renderPass) override;

		void RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline) override;

		void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline) override;
		void RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets) override;

		void RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets) override;
		void RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* materal) override;


	private:
	
	};
}
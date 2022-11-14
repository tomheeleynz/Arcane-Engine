#include "OpenGLRenderer.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Arcane 
{
	OpenGLRenderer::OpenGLRenderer()
	{

	}

	void OpenGLRenderer::Shutdown()
	{

	}

	void OpenGLRenderer::BeginFrame()
	{
		// Not Needed
	}

	void OpenGLRenderer::EndFrame()
	{
		// Not Needed
	}

	void OpenGLRenderer::BeginRenderPass(RenderPass* renderPass)
	{
		if (renderPass->GetRenderPassSpecs().SwapchainFramebuffer)
			return;

		OpenGLFramebuffer* framebuffer = static_cast<OpenGLFramebuffer*>(renderPass->GetRenderPassSpecs().TargetFramebuffer);
		framebuffer->Bind();
	}

	void OpenGLRenderer::EndRenderPass(RenderPass* renderPass)
	{
		if (renderPass->GetRenderPassSpecs().SwapchainFramebuffer)
			return;
		
		OpenGLFramebuffer* framebuffer = static_cast<OpenGLFramebuffer*>(renderPass->GetRenderPassSpecs().TargetFramebuffer);
		framebuffer->UnBind();
	}

	void OpenGLRenderer::RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline)
	{

	}

	void OpenGLRenderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline)
	{

	}

	void OpenGLRenderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets)
	{
	}

	void OpenGLRenderer::RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets)
	{
	}

	void OpenGLRenderer::RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* materal)
	{
	}
}
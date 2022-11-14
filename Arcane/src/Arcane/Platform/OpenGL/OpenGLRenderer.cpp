#include "OpenGLRenderer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLPipeline.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexDescriptor.h"

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
		if (renderPass->GetRenderPassSpecs().SwapchainFramebuffer) {
			OpenGLFramebuffer* framebuffer = static_cast<OpenGLFramebuffer*>(renderPass->GetRenderPassSpecs().TargetFramebuffer);
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(
				framebuffer->GetSpecs().ClearColor.x,
				framebuffer->GetSpecs().ClearColor.y,
				framebuffer->GetSpecs().ClearColor.z,
				framebuffer->GetSpecs().ClearColor.w
			);
		}
		else {
			OpenGLFramebuffer* framebuffer = static_cast<OpenGLFramebuffer*>(renderPass->GetRenderPassSpecs().TargetFramebuffer);
			framebuffer->Bind();

			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(
				framebuffer->GetSpecs().ClearColor.x,
				framebuffer->GetSpecs().ClearColor.y,
				framebuffer->GetSpecs().ClearColor.z,
				framebuffer->GetSpecs().ClearColor.w
			);
		}
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
		OpenGLVertexBuffer* vertexBuffer = static_cast<OpenGLVertexBuffer*>(buffer);
		OpenGLPipeline* openglPipeline = static_cast<OpenGLPipeline*>(pipeline);
		OpenGLVertexDescriptor* vertexDescriptor = static_cast<OpenGLVertexDescriptor*>(openglPipeline->GetSpec().descriptor);

		// Bind Shader
		openglPipeline->BindShader();

		// Bind VAO
		if (!vertexDescriptor->GetIsGenerated()) {
			vertexDescriptor->Generate(buffer);
		}

		openglPipeline->BindVertexDescriptor();

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Unbind Everyting
		openglPipeline->UnbindVertexDescriptor();
		openglPipeline->UnbindShader();
		vertexBuffer->UnBind();
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
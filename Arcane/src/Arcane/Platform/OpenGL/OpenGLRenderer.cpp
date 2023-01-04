#include "OpenGLRenderer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLPipeline.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexDescriptor.h"
#include "OpenGLDescriptorSet.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Arcane 
{
	OpenGLRenderer::OpenGLRenderer()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

			glClearColor(
				framebuffer->GetSpecs().ClearColor.x,
				framebuffer->GetSpecs().ClearColor.y,
				framebuffer->GetSpecs().ClearColor.z,
				framebuffer->GetSpecs().ClearColor.w
			);

			glClear(GL_COLOR_BUFFER_BIT);
			glViewport(0, 0, 1600, 1200);
		}
		else {
			OpenGLFramebuffer* framebuffer = static_cast<OpenGLFramebuffer*>(renderPass->GetRenderPassSpecs().TargetFramebuffer);
			framebuffer->Bind();
			glClearColor(
				framebuffer->GetSpecs().ClearColor.x,
				framebuffer->GetSpecs().ClearColor.y,
				framebuffer->GetSpecs().ClearColor.z,
				framebuffer->GetSpecs().ClearColor.w
			);

			glClear(GL_COLOR_BUFFER_BIT);
			glViewport(0, 0, framebuffer->GetSpecs().Width, framebuffer->GetSpecs().Height);
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
		OpenGLVertexBuffer* vertexBuffer = static_cast<OpenGLVertexBuffer*>(buffer);
		OpenGLPipeline* openglPipeline = static_cast<OpenGLPipeline*>(pipeline);
		OpenGLVertexDescriptor* vertexDescriptor = static_cast<OpenGLVertexDescriptor*>(openglPipeline->GetSpec().descriptor);
		OpenGLIndexBuffer* indexBuffer = static_cast<OpenGLIndexBuffer*>(vertexBuffer->GetIndexBuffer());
		OpenGLDescriptorSet* descriptorSet = static_cast<OpenGLDescriptorSet*>(openglPipeline->GetSpec().DescriptorSets[0]);
		OpenGLShader* shader = static_cast<OpenGLShader*>(openglPipeline->GetSpec().shader);

		// Bind Shader
		openglPipeline->BindShader();

		// Bind VAO
		if (!vertexDescriptor->GetIsGenerated()) {
			vertexDescriptor->Generate(buffer);
		}

		openglPipeline->BindVertexDescriptor();
		indexBuffer->Bind();

		// Bind Descriptor Sets
		descriptorSet->BindTextures(shader->GetShaderID());

		// Draw
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

		// Unbind Everyting
		openglPipeline->UnbindVertexDescriptor();
		openglPipeline->UnbindShader();
		vertexBuffer->UnBind();
		indexBuffer->UnBind();
	}

	void OpenGLRenderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets)
	{
		OpenGLVertexBuffer* vertexBuffer = static_cast<OpenGLVertexBuffer*>(buffer);
		OpenGLPipeline* openglPipeline = static_cast<OpenGLPipeline*>(pipeline);
		OpenGLVertexDescriptor* vertexDescriptor = static_cast<OpenGLVertexDescriptor*>(openglPipeline->GetSpec().descriptor);
		OpenGLIndexBuffer* indexBuffer = static_cast<OpenGLIndexBuffer*>(vertexBuffer->GetIndexBuffer());
		OpenGLShader* shader = static_cast<OpenGLShader*>(openglPipeline->GetSpec().shader);

		// Bind Shader
		openglPipeline->BindShader();

		// Bind VAO
		if (!vertexDescriptor->GetIsGenerated()) {
			vertexDescriptor->Generate(buffer);
		}

		openglPipeline->BindVertexDescriptor();
		indexBuffer->Bind();

		// Bind Descriptor Sets
		for (int i = 0; i < descriptorSets.size(); i++) {
			OpenGLDescriptorSet* descriptorSet = static_cast<OpenGLDescriptorSet*>(descriptorSets[i]);
			descriptorSet->BindTextures(shader->GetShaderID());
		}

		// Draw
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

		// Unbind Everyting
		openglPipeline->UnbindVertexDescriptor();
		openglPipeline->UnbindShader();
		vertexBuffer->UnBind();
		indexBuffer->UnBind();
	}

	void OpenGLRenderer::RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets)
	{
	}

	void OpenGLRenderer::RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* materal)
	{
	}
}
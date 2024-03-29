#include "Renderer.h"

// -- Vulkan Renderer
#include "Arcane/Platform/Vulkan/VulkanRenderer.h"

// -- OpenGL Renderer
#include "Arcane/Platform/OpenGL/OpenGLRenderer.h"

namespace Arcane {
	static RendererAPI* s_RendererAPI = nullptr;

	void RendererAPI::SetAPI(RendererAPIType api)
	{
		s_CurrentRendererAPI = api;
	}

	static RendererAPI* InitRendererAPI()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::Vulkan: return new VulkanRenderer();
		case RendererAPIType::OpenGL: return new OpenGLRenderer();
		}
	}

	void Renderer::Init()
	{
		s_RendererAPI = InitRendererAPI();
	}

	void Renderer::Shutdown()
	{
		s_RendererAPI->Shutdown();
	}

	void Renderer::BeginFrame()
	{
		s_RendererAPI->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		s_RendererAPI->EndFrame();
	}

	void Renderer::BeginRenderPass(RenderPass* renderPass)
	{
		s_RendererAPI->BeginRenderPass(renderPass);
	}

	void Renderer::EndRenderPass(RenderPass* renderPass)
	{
		s_RendererAPI->EndRenderPass(renderPass);
	}

	void Renderer::RenderTriangle(VertexBuffer* buffer, Pipeline* pipeline)
	{
		s_RendererAPI->RenderTriangle(buffer, pipeline);
	}

	void Renderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline)
	{
		s_RendererAPI->RenderQuad(buffer, pipeline);
	}

	void Renderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets)
	{
		s_RendererAPI->RenderQuad(buffer, pipeline, descriptorSets);
	}

	void Renderer::RenderMesh(VertexBuffer* buffer, Pipeline* pipeline, std::vector<DescriptorSet*> descriptorSets)
	{
		s_RendererAPI->RenderMesh(buffer, pipeline, descriptorSets);
	}

	void Renderer::RenderMeshWithMaterial(VertexBuffer* buffer, Pipeline* pipeline, Material* material)
	{

	}
}
#include "Renderer.h"

// -- Vulkan Renderer
#ifdef ARCANE_WIN32
#include "Arcane/Platform/Vulkan/VulkanRenderer.h"
#endif

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
#ifdef ARCANE_WIN32
		case RendererAPIType::Vulkan: return new VulkanRenderer();
#endif
        default: return nullptr;
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

	void Renderer::RenderQuad(VertexBuffer* buffer, Pipeline* pipeline, UniformBuffer* uniformBuffer)
	{
		s_RendererAPI->RenderQuad(buffer, pipeline, uniformBuffer);
	}
}

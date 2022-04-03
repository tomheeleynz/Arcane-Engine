#include "SceneRenderer.h"

namespace Arcane
{
	struct SceneRendererData
	{
		// Composite Render Pass
		Framebuffer* CompositeFramebuffer;
		RenderPass* CompositeRenderPass;

		// Meshes to be rendererd
		std::vector<Mesh*> m_Meshes;
	};

	static SceneRendererData s_Data;

	SceneRenderer::SceneRenderer()
	{
		// Setup Composite Renderpass
		FramebufferSpecifications compositeFramebufferSpecs;
		
		compositeFramebufferSpecs.AttachmentSpecs = {
			FramebufferAttachmentType::COLOR,
			FramebufferAttachmentType::DEPTH
		};

		compositeFramebufferSpecs.ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
		compositeFramebufferSpecs.Width = 512;
		compositeFramebufferSpecs.Height = 512;
		s_Data.CompositeFramebuffer = Arcane::Framebuffer::Create(compositeFramebufferSpecs);

		RenderPassSpecs compositeSpecs;
		compositeSpecs.SwapchainFramebuffer = false;
		compositeSpecs.TargetFramebuffer = s_Data.CompositeFramebuffer;
		s_Data.CompositeRenderPass = Arcane::RenderPass::Create(compositeSpecs);
	}

	Framebuffer* SceneRenderer::GetFinalRenderFramebuffer()
	{
		return s_Data.CompositeFramebuffer;
	}

	void SceneRenderer::CompositeRenderPass()
	{
		Renderer::BeginRenderPass(s_Data.CompositeRenderPass);
		{
			
		}
		Renderer::EndRenderPass(s_Data.CompositeRenderPass);
	}

	void SceneRenderer::RenderScene()
	{
		CompositeRenderPass();
	}

	void SceneRenderer::SubmitMesh(Mesh* mesh)
	{
		s_Data.m_Meshes.push_back(mesh);
	}
}
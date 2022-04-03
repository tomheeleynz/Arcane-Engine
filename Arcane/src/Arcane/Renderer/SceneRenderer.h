#pragma once

#include "Framebuffer.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "VertexDescriptor.h"
#include "Renderer.h"
#include "Mesh.h"

namespace Arcane
{
	class SceneRenderer
	{
	public:
		SceneRenderer();

		void CompositeRenderPass();
		Framebuffer* GetFinalRenderFramebuffer();

		void RenderScene();
		void SubmitMesh(Mesh* mesh);
	private:

	};
}
#pragma once

#include <glm/glm.hpp>


#include "Framebuffer.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "VertexDescriptor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "UniformBuffer.h"

namespace Arcane
{
	class SceneRenderer
	{
	public:
		SceneRenderer();


		Framebuffer* GetFinalRenderFramebuffer();

		void RenderScene();

		void SubmitMesh(Mesh* mesh);
	private:
		void CompositeRenderPass();
		void GeometryPass();
	};
}
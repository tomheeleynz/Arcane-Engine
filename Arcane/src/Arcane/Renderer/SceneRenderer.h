#pragma once

#include <glm/glm.hpp>


#include "Arcane/ECS/Component.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "VertexDescriptor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "UniformBuffer.h"
#include "Camera.h"

namespace Arcane
{
	class SceneRenderer
	{
	public:
		SceneRenderer();


		Framebuffer* GetFinalRenderFramebuffer();

		void RenderScene();

		void SetCamera(Camera* camera);
		void SubmitMesh(Mesh* mesh, TransformComponent& component);
	private:
		void CompositeRenderPass();
		void GeometryPass();
	};
}
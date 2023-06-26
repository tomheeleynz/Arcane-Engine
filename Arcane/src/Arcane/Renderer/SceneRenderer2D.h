#pragma once

#include "Camera.h"
#include "Arcane/ECS/Component.h"
#include "Quad.h"

namespace Arcane
{
	class SceneRenderer2D
	{
	public:
		SceneRenderer2D();

		void RenderScene();

		void SetCamera(Camera* camera);

		void ResizeScene(uint32_t width, uint32_t height);
		glm::vec2 GetSceneSize() { return m_SceneSize; }

		// Render Quad
		void SubmitQuad(Quad* quad, TransformComponent& component, glm::vec3 color, Material* material);

		Framebuffer* GetFinalRenderFramebuffer();
	private:
		void CompositeRenderPass();
		void GeometryPass();
		glm::vec2 m_SceneSize;
	};
}
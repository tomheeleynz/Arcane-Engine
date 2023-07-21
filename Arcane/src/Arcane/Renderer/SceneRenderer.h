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
#include "DescriptorSet.h"
#include "Material.h"

namespace Arcane
{
	class SceneRenderer
	{
	public:
		SceneRenderer();

		Framebuffer* GetFinalRenderFramebuffer();

		void RenderScene();

		void SetCamera(Camera* camera);
		void SubmitMesh(Mesh* mesh, TransformComponent& component, Material* material);
		void SubmitQuad(TransformComponent& transformComponent, SpriteRendererComponent& spriteRendererComponent);

		// Lighting code
		void SetDirectionalLight(LightComponent& light, TransformComponent& transform);

		void ResizeScene(uint32_t width, uint32_t height);

		glm::vec2 GetSceneSize() { return m_SceneSize; }

		void RenderGrid(bool render) { m_RenderGrid = render; }
	private:
		void CompositeRenderPass();
		void GeometryPass();
		glm::vec2 m_SceneSize;

		bool m_RenderGrid = true;
	};
}
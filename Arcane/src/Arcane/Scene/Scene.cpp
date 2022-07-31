#include <iostream>
#include "Scene.h"
#include "Arcane/ECS/Entity.h"

namespace Arcane
{
	Scene::Scene()
	{
		m_SceneRenderer = nullptr;
	}

	Entity* Scene::CreateEntity(std::string name)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);
		newEntity->AddComponent<TagComponent>(name);
		return newEntity;
	}

	void Scene::OnUpdate()
	{
		// Render Mesh
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, MeshRendererComponent>();
			for (auto& entity : view) 
			{
				auto& mesh = view.get<MeshComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);
				auto& meshRenderer = view.get<MeshRendererComponent>(entity);
				m_SceneRenderer->SubmitMesh(mesh.mesh, transform, meshRenderer.material);
			}
		}
		m_SceneRenderer->RenderScene();
	}
}
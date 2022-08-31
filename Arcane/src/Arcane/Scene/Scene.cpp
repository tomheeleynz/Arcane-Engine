#include <iostream>
#include "Scene.h"
#include "Arcane/ECS/Entity.h"

namespace Arcane
{
	Scene::Scene()
	{
		m_SceneRenderer = nullptr;

		// Add defaults to scene

		// -- Directional Light
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);
		newEntity->AddComponent<TagComponent>("Directional Light");
		newEntity->AddComponent<TransformComponent>();
		newEntity->AddComponent<LightComponent>(LightType::DIRECTIONAL, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	Entity* Scene::CreateEntity(std::string name)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);
		newEntity->AddComponent<TagComponent>(name);
		newEntity->AddComponent<TransformComponent>();
		return newEntity;
	}

	void Scene::OnUpdate()
	{
		// Add Lights to scene
		{
			auto view = m_Registry.view<TransformComponent, LightComponent>();

			for (auto& entity : view) 
			{
				auto& light = view.get<LightComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);
				
				if (light.type == LightType::DIRECTIONAL) 
				{
					m_SceneRenderer->SetDirectionalLight(light, transform);
				}
			}
		}

		// Render Mesh
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, MeshRendererComponent>();
			for (auto& entity : view) 
			{
				auto& mesh = view.get<MeshComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);
				auto& meshRenderer = view.get<MeshRendererComponent>(entity);

				if (mesh.mesh != nullptr)
					m_SceneRenderer->SubmitMesh(mesh.mesh, transform, meshRenderer.material);
			}
		}
		m_SceneRenderer->RenderScene();
	}
}
#include <iostream>
#include "Scene.h"
#include "Arcane/ECS/Entity.h"

namespace Arcane
{
	Scene::Scene(bool newScene)
	{
		m_SceneRenderer = nullptr;

		if (newScene) {
			// Add a directional light to scene
			entt::entity newHandle = m_Registry.create();
			Entity* newEntity = new Entity(newHandle, this);

			newEntity->AddComponent<IDComponent>();
			newEntity->AddComponent<TagComponent>("Directional Light");
			newEntity->AddComponent<TransformComponent>();
			newEntity->AddComponent<LightComponent>(LightType::DIRECTIONAL, glm::vec3(5.0f, 2.0f, 0.0f));
		}
	}

	Entity* Scene::CreateEntity(std::string name)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);

		newEntity->AddComponent<TagComponent>(name);
		newEntity->AddComponent<IDComponent>();
		newEntity->AddComponent<TransformComponent>();

		return newEntity;
	}

	Entity* Scene::CreateEntityWithUUID(std::string name, uint64_t uuid)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);

		newEntity->AddComponent<TagComponent>(name);
		newEntity->AddComponent<IDComponent>(uuid);
		newEntity->AddComponent<TransformComponent>();

		return newEntity;
	}

	void Scene::DeleteEntity(Entity& entity)
	{
		entity.DeleteEntity();
	}

	void Scene::OnUpdate(float deltaTime)
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

		// Run Update Scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto& entity : view) {
				auto& scriptComponent = view.get<ScriptComponent>(entity);
				
				if (scriptComponent.script != nullptr) {
					Script* script = scriptComponent.script;
					script->OnUpdate(deltaTime);
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

				if (mesh.mesh != nullptr && meshRenderer.material != nullptr && meshRenderer.material->GetShader() != nullptr)
					m_SceneRenderer->SubmitMesh(mesh.mesh, transform, meshRenderer.material);
			}
		}
		
		m_SceneRenderer->RenderScene();
	}
}
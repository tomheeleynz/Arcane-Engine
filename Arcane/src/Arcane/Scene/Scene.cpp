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

		// Create Physics Scene to go along with scene
		physx::PxSceneDesc sceneDesc(PhysicsEngine::GetPhysics()->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = PhysicsEngine::GetDispatcher();
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_PhysicsScene = PhysicsEngine::GetInstance()->GetPhysics()->createScene(sceneDesc);
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

		// Do Physics Update Here
		m_PhysicsScene->simulate(1.0f / 60.0f);
		m_PhysicsScene->fetchResults(true);

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
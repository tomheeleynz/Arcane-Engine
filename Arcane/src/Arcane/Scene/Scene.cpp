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

		m_SceneRenderer->RenderGrid(true);
		m_SceneRenderer->RenderScene();
	}

	void Scene::OnRuntimeUpdate(float deltaTime)
	{
		// Get Primary Camera in scene to render to
		Camera* camera = nullptr;
		{
			auto view = m_Registry.view<CameraComponent, TransformComponent>();

			for (auto& entity : view) {
				auto& cameraComponent = m_Registry.get<CameraComponent>(entity);
				auto& transformComponent = m_Registry.get<TransformComponent>(entity);

				ViewData newData;
				newData.CameraPosition = transformComponent.pos;
				newData.CameraLookDir = { 0.0f, 0.0f, 0.0f };
				newData.CameraUpDir = { 0.0f, 1.0f, 0.0f };

				if (cameraComponent.isPrimary) {
					if (cameraComponent.type == CameraType::Orthographic) {
						if (cameraComponent.orthoCamera == nullptr)
							cameraComponent.orthoCamera = new OrthoCamera(m_SceneRenderer->GetSceneSize().x, m_SceneRenderer->GetSceneSize().y);

						cameraComponent.orthoCamera->SetViewData(newData);
						camera = cameraComponent.orthoCamera;
					}
					else {
						if (cameraComponent.perspectiveCamera == nullptr)
							cameraComponent.perspectiveCamera = new PerspectiveCamera(m_SceneRenderer->GetSceneSize().x, m_SceneRenderer->GetSceneSize().y, 45.0f);
						cameraComponent.perspectiveCamera->SetViewData(newData);
						camera = cameraComponent.perspectiveCamera;
					}
				}
			}
		}

		if (camera != nullptr) {
			m_SceneRenderer->SetCamera(camera);
		}

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

		// Update position with physics
		{
			auto view = m_Registry.view<RigidBodyComponent, TransformComponent>();
			for (auto& entity : view) {
				auto& rigidBodyComponent = view.get<RigidBodyComponent>(entity);
				auto& transformComponent = view.get<TransformComponent>(entity);

				transformComponent.pos = rigidBodyComponent.rigidBody->GetPosition();
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

		m_SceneRenderer->RenderGrid(false);
		m_SceneRenderer->RenderScene();
	}
}
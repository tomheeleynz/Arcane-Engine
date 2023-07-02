#include <iostream>
#include "Scene.h"
#include "Arcane/ECS/Entity.h"
#include "Arcane/Core/Application.h"

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

		m_PhysicsWorld = new Kinetics::World(-9.8f);
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

	void Scene::OnStart() 
	{
		// Run start function of scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto& entity : view) {
				auto& scriptComponent = view.get<ScriptComponent>(entity);

				if (scriptComponent.script != nullptr) {
					Script* script = scriptComponent.script;
					script->OnStart();
				}
			}
		}
	}

	void Scene::OnUpdate(float deltaTime)
	{
		if (Application::Get().GetProject()->GetDimensionType() == DimensionType::TwoD)
		{
			auto view = m_Registry.view<TransformComponent, SpriteRenderer>();
			for (auto& entity : view)
			{
				auto& spriteRenderer = view.get<SpriteRenderer>(entity);
				auto& transform = view.get<TransformComponent>(entity);

				if (spriteRenderer.material != nullptr && spriteRenderer.material->GetShader() != nullptr)
					m_SceneRenderer2D->SubmitQuad(spriteRenderer.quad, transform, {1.0f, 1.0f, 1.0f}, spriteRenderer.material);
			}
			m_SceneRenderer2D->RenderScene();
		}
		else {
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

		// Update Physics Engine
		m_PhysicsWorld->Step(1.0f / 60.0f);

		// Render Mesh
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, MeshRendererComponent>();
			for (auto& entity : view)
			{
				auto& mesh = view.get<MeshComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);
				auto& meshRenderer = view.get<MeshRendererComponent>(entity);

				Entity entityHandle = Entity(entity, this);

				if (entityHandle.HasComponent<RigidBody>()) {
					transform.pos = {
						entityHandle.GetComponent<RigidBody>().body->GetPosition().x,
						entityHandle.GetComponent<RigidBody>().body->GetPosition().y,
						entityHandle.GetComponent<RigidBody>().body->GetPosition().z,
					};
				}

				if (mesh.mesh != nullptr && meshRenderer.material != nullptr && meshRenderer.material->GetShader() != nullptr)
					m_SceneRenderer->SubmitMesh(mesh.mesh, transform, meshRenderer.material);
			}
		}

		m_SceneRenderer->RenderGrid(false);
		m_SceneRenderer->RenderScene();
	}
	void Scene::SetSceneCamera(Camera* sceneCamera)
	{
		if (Application::Get().GetProject()->GetDimensionType() == DimensionType::TwoD)
			m_SceneRenderer2D->SetCamera(sceneCamera);
		else 
			m_SceneRenderer->SetCamera(sceneCamera);
	}

	Kinetics::DynamicBody* Scene::AddDynamicBodyToPhysicsWorld(Kinetics::ShapeDef shapeDef, Kinetics::BodyDef bodyDef)
	{
		Kinetics::DynamicBody* newBody = m_PhysicsWorld->CreateDynamicBody(shapeDef, bodyDef);
		return newBody;
	}
}
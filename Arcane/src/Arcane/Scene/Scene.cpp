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

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, std::unordered_map<Core::UUID, entt::entity> enttMap)
	{
		auto view = src.view<Component>();
		for (auto srcEntity : view)
		{
			entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).uuid);

			auto& srcComponent = src.get<Component>(srcEntity);
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);
		}
	}

	static void AddPhysicsObjectsToScene(entt::registry& src, Scene* scene) {
	}

	Scene* Scene::Copy(Scene* other)
	{
		Scene* newScene = new Scene(false);
		
		auto& srcSceneRegistery = other->m_Registry;
		auto& dstSceneRegistery = newScene->m_Registry;
		auto& idView = srcSceneRegistery.view<IDComponent>();

		std::unordered_map<Core::UUID, entt::entity> enttMap;

		for (auto& e : idView) 
		{
			Core::UUID uuid = srcSceneRegistery.get<IDComponent>(e).uuid;
			std::string name = srcSceneRegistery.get<TagComponent>(e).tag;
			Entity& entity = *newScene->CreateEntityWithUUID(name, (uint64_t)uuid);
			enttMap[uuid] = (entt::entity)entity;
		}

		CopyComponent<TransformComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);
		CopyComponent<MeshComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);
		CopyComponent<MeshRendererComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);
		CopyComponent<LightComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);
		CopyComponent<ScriptComponent>(dstSceneRegistery, srcSceneRegistery, enttMap);

		AddPhysicsObjectsToScene(dstSceneRegistery, newScene);
		return newScene;
	}

	Entity* Scene::CreateEntity(std::string name)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);
		Core::UUID newUUID = Core::UUID();

		newEntity->AddComponent<TagComponent>(name);
		newEntity->AddComponent<IDComponent>(newUUID);
		newEntity->AddComponent<TransformComponent>();

		m_EntityMap[newUUID] = newHandle;

		return newEntity;
	}

	Entity* Scene::CreateEntityWithUUID(std::string name, uint64_t uuid)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);

		newEntity->AddComponent<TagComponent>(name);
		newEntity->AddComponent<IDComponent>(uuid);
		newEntity->AddComponent<TransformComponent>();

		m_EntityMap[uuid] = newHandle;

		return newEntity;
	}

	Entity Scene::GetEntityByUUID(Core::UUID uuid) {
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
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

		// Sprite Render Component
		{
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			
			for (auto& entity : view) 
			{
				auto& spriteRendererComponent = view.get<SpriteRendererComponent>(entity);
				auto& transformComponent = view.get<TransformComponent>(entity);
				
				Entity entityHandle = Entity(entity, this);

				if (entityHandle.HasComponent<Animator>())
					m_SceneRenderer->SubmitAnimatedQuad(transformComponent, spriteRendererComponent, entityHandle.GetComponent<Animator>(), false);
				else
					m_SceneRenderer->SubmitQuad(transformComponent, spriteRendererComponent);
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

		// m_SceneRenderer->RenderGrid(true);
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
						if (cameraComponent.orthoCamera == nullptr) {
							cameraComponent.orthoCamera = new OrthoCamera(m_SceneRenderer->GetSceneSize().x, m_SceneRenderer->GetSceneSize().y);
						}

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
			ScriptingEngine::SetSceneContext(this);
			
			for (auto& entity : view) {
				auto& scriptComponent = view.get<ScriptComponent>(entity);

				if (scriptComponent.script != nullptr) {
					Script* script = scriptComponent.script;
		
					if (scriptComponent.updateProperties == true) {
						script->WriteProperties();
						scriptComponent.updateProperties = false;
						m_Registry.emplace_or_replace<ScriptComponent>(entity, scriptComponent);
					}

					script->OnUpdate(deltaTime);
				}
			}
		}

		// Update Gravity Scale
		{
			auto view = m_Registry.view<RigidBodyComponent2D>();

			for (auto& entity : view)
			{
				auto& rigidBody2D = m_Registry.get<RigidBodyComponent2D>(entity);
				rigidBody2D.body->SetGravityScale(rigidBody2D.gravityScale);
			}
		}

		// Update Physics Engine
		m_PhysicsWorld->Step(1.0f / 60.0f);

		// Sprite Render Component
		{
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

			for (auto& entity : view)
			{
				auto& spriteRendererComponent = view.get<SpriteRendererComponent>(entity);
				auto& transformComponent = view.get<TransformComponent>(entity);

				Entity entityHandle = Entity(entity, this);

				if (entityHandle.HasComponent<RigidBodyComponent2D>()) {
					transformComponent.pos = {
						entityHandle.GetComponent<RigidBodyComponent2D>().body->GetPosition().x,
						entityHandle.GetComponent<RigidBodyComponent2D>().body->GetPosition().y,
						0.0f
					};

					if (entityHandle.HasComponent<Animator>())
						m_SceneRenderer->SubmitAnimatedQuad(transformComponent, spriteRendererComponent, entityHandle.GetComponent<Animator>(), true);
					else
						m_SceneRenderer->SubmitQuad(transformComponent, spriteRendererComponent);
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

				Entity entityHandle = Entity(entity, this);

				if (mesh.mesh != nullptr && meshRenderer.material != nullptr && meshRenderer.material->GetShader() != nullptr)
					m_SceneRenderer->SubmitMesh(mesh.mesh, transform, meshRenderer.material);
			}
		}

		m_SceneRenderer->RenderGrid(false);
		m_SceneRenderer->RenderScene();
	}

	void Scene::OnRuntimeStart()
	{
		{
			// Run start scripts
			auto view = m_Registry.view<ScriptComponent>();

			for (auto& e : view) {
				auto& scriptComponent = view.get<ScriptComponent>(e);
				scriptComponent.script->OnStart();
			}
		}
	}

	void Scene::SetSceneCamera(Camera* sceneCamera)
	{
		m_SceneRenderer->SetCamera(sceneCamera);
	}

	Kinetics::DynamicBody2D* Scene::AddDynamicBodyToPhysicsWorld(Kinetics::BodyDef bodyDef)
	{
		return m_PhysicsWorld->CreateDynamicBody2D(bodyDef);
	}

	void Scene::CopyDynamicBodyToPhysicsWorld(Kinetics::DynamicBody* body)
	{
		m_PhysicsWorld->AddDynamicBody(body);
	}
}
#pragma once

#include <string>
#include <entt/entt.hpp>
#include <Kinetics/Core/World.h>

#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/SceneRenderer.h"
#include "Arcane/Renderer/SceneRenderer2D.h"
#include "Arcane/Assets/Asset.h"
#include "Arcane/Physics/PhysicsEngine.h"

namespace Arcane
{
	class Entity;

	class Scene : public Asset
	{
	public:
		Scene(bool newScene);

		Entity* CreateEntity(std::string name);
		Entity* CreateEntityWithUUID(std::string name, uint64_t uuid);

		void DeleteEntity(Entity& entity);

		void OnStart();
		void OnUpdate(float deltaTime);
		void OnRuntimeUpdate(float deltaTime);

		void SetSceneName(std::string name) { m_Name = name; }
		std::string GetSceneName() { return m_Name;  }
		
		// Making Registry Public for ease
		entt::registry m_Registry;

		void SetSceneRenderer(SceneRenderer* sceneRenderer) { m_SceneRenderer = sceneRenderer; }
		void SetSceneRenderer2D(SceneRenderer2D* sceneRenderer) { m_SceneRenderer2D = sceneRenderer; }
		void SetSceneCamera(Camera* sceneCamera);

		void AddToPhyicsScene(physx::PxActor* actor) { m_PhysicsScene->addActor(*actor); }
	private:
		std::string m_Name;
		SceneRenderer* m_SceneRenderer;
		SceneRenderer2D* m_SceneRenderer2D;
		physx::PxScene* m_PhysicsScene;
		Kinetics::World* m_SceneWorld;
		friend class Entity;
	};
}
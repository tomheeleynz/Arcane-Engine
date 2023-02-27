#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/SceneRenderer.h"
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

		void OnUpdate();

		void SetSceneName(std::string name) { m_Name = name; }
		std::string GetSceneName() { return m_Name;  }
		
		// Making Registry Public for ease
		entt::registry m_Registry;

		void SetSceneRenderer(SceneRenderer* sceneRenderer) { m_SceneRenderer = sceneRenderer; }
		void SetSceneCamera(Camera* sceneCamera) { m_SceneRenderer->SetCamera(sceneCamera); }
	private:
		std::string m_Name;
		SceneRenderer* m_SceneRenderer;
		physx::PxScene* m_PhysicsScene;
		friend class Entity;
	};
}
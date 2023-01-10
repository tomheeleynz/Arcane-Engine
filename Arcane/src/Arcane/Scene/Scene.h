#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/SceneRenderer.h"

namespace Arcane
{
	class Entity;

	class Scene
	{
	public:
		Scene();

		Entity* CreateEntity(std::string name);
		Entity* CreateEntityWithUUID(std::string name, uint64_t uuid);

		void DeleteEntity(Entity& entity);

		void OnUpdate();

		void SetName(std::string name) { m_Name = name; }
		std::string GetName() { return m_Name;  }
		
		// Making Registry Public for ease
		entt::registry m_Registry;

		void SetSceneRenderer(SceneRenderer* sceneRenderer) { m_SceneRenderer = sceneRenderer; }
		void SetSceneCamera(Camera* sceneCamera) { m_SceneRenderer->SetCamera(sceneCamera); }
	private:
		std::string m_Name;
		SceneRenderer* m_SceneRenderer;
		friend class Entity;
	};
}
#pragma once

#include <string>
#include <entt/entt.hpp>
#include <Kinetics/Core/World.h>
#include <map>
#include <unordered_map>

#include "Arcane/ECS/Component.h"
#include "Arcane/Renderer/SceneRenderer.h"
#include "Arcane/Assets/Asset.h"

namespace Arcane
{
	class Entity;

	class Scene : public Asset
	{
	public:
		Scene(bool newScene);

		Entity* CreateEntity(std::string name);
		Entity* CreateEntityWithUUID(std::string name, uint64_t uuid);

		Entity GetEntityByUUID(Core::UUID uuid);

		static Scene* Copy(Scene* other);

		void DeleteEntity(Entity& entity);

		void OnStart();
		void OnUpdate(float deltaTime);
		void OnRuntimeUpdate(float deltaTime);

		void OnRuntimeStart();

		void SetSceneName(std::string name) { m_Name = name; }
		std::string GetSceneName() { return m_Name;  }

		// Making Registry Public for ease
		entt::registry m_Registry;

		void SetSceneRenderer(SceneRenderer* sceneRenderer) { m_SceneRenderer = sceneRenderer; }
		void SetSceneCamera(Camera* sceneCamera);

		Kinetics::DynamicBody* AddDynamicBodyToPhysicsWorld(Kinetics::BodyDef bodyDef);
		void CopyDynamicBodyToPhysicsWorld(Kinetics::DynamicBody* body);
	private:
		std::string m_Name;
		SceneRenderer* m_SceneRenderer;
		Kinetics::World* m_PhysicsWorld;

		std::unordered_map<Core::UUID, entt::entity> m_EntityMap;

		friend class Entity;
	};
}
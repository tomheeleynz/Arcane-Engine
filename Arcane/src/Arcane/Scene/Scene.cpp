#include <iostream>
#include "Scene.h"
#include "Arcane/ECS/Entity.h"

namespace Arcane
{
	Scene::Scene()
	{
	}

	Entity* Scene::CreateEntity(std::string name)
	{
		entt::entity newHandle = m_Registry.create();
		Entity* newEntity = new Entity(newHandle, this);
		newEntity->AddComponent<TagComponent>(name);
		return newEntity;
	}

	void Scene::OnUpdate()
	{
	}
}
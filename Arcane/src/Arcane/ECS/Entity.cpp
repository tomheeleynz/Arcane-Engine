#include "Entity.h"

namespace Arcane
{
	Entity::Entity(entt::entity handle, Scene* scene)
	{
		m_EntityHandle = handle;
		m_Scene = scene;
	}
}
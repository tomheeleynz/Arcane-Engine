#pragma once

#include <entt/entt.hpp>

#include "Arcane/Scene/Scene.h"

namespace Arcane
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);


	private:
		entt::entity m_EntityHandle { entt::null };
		Scene* m_Scene = nullptr;
	};
}
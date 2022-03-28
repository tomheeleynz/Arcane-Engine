#pragma once

#include <string>
#include <entt/entt.hpp>


#include "Arcane/ECS/Component.h"

namespace Arcane
{
	class Entity;

	class Scene
	{
	public:
		Scene();

		Entity* CreateEntity(std::string name);

		void OnUpdate();

		void SetName(std::string name) { m_Name = name; }
		std::string GetName() { return m_Name;  }
	private:
		std::string m_Name;
		entt::registry m_Registry;
		friend class Entity;
	};
}
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
		
		// Making Registry Public for ease
		entt::registry m_Registry;
	private:
		std::string m_Name;
		friend class Entity;
	};
}
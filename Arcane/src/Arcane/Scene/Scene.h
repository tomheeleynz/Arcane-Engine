#pragma once

#include <string>
#include <entt/entt.hpp>

namespace Arcane
{
	class Entity;

	class Scene
	{
	public:
		Scene(std::string name);

		Entity* CreateEntity(std::string name);

		void OnUpdate();
	private:
		std::string m_Name;
		entt::registry m_Registry;
	};
}
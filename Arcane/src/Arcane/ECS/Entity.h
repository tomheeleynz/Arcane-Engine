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

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) 
		{
			if (!HasComponent<T>())
			{
				T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
				return component;
			}
		}

		template <typename T>
		bool HasComponent() 
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template <typename T>
		T& GetComponent() 
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		void DeleteEntity()
		{
			m_Scene->m_Registry.destroy(m_EntityHandle);
		}

		Scene* GetScene() { return m_Scene; }

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
	private:
		entt::entity m_EntityHandle { entt::null };
		Scene* m_Scene = nullptr;
	};
}
#include <iostream>

#include "ScriptingEngine.h"
#include "Arcane/Core/InputManager.h"
#include "Arcane/ECS/Entity.h"

#include "ScriptGlue.h"

namespace Arcane
{
	static void PrintStack(lua_State* L)
	{
		int top = lua_gettop(L);


		std::string str = "From top to bottom, the lua stack is \n";
		for (unsigned index = top; index > 0; index--)
		{
			int type = lua_type(L, index);
			switch (type)
			{
				// booleans
			case LUA_TBOOLEAN:
				str = str + (lua_toboolean(L, index) ? "true" : "false") + "\n";
				break;

				// numbers
			case LUA_TNUMBER:
				str = str + std::to_string(lua_tonumber(L, index)) + "\n";
				break;

				// strings
			case LUA_TSTRING:
				str = str + lua_tostring(L, index) + "\n";
				break;

				// other
			default:
				str = str + lua_typename(L, type) + "\n";
				break;
			}
		}

		str = str + "\n";
		std::cout << str;
	}

	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		m_LuaState = luaL_newstate();
		luaL_openlibs(m_LuaState);

		ScriptGlue::RegisterFunctions(m_LuaState);
		ScriptGlue::RegisterMetatables(m_LuaState);
		ScriptGlue::RegisterTables(m_LuaState);
	}

	void ScriptingEngine::SetSceneContext(Scene* scene)
	{
		GetInstance()->SetSceneContextImpl(scene);
	}

	Scene* ScriptingEngine::GetSceneContext()
	{
		return GetInstance()->GetSceneContextImpl();
	}

	int ScriptingEngine::GetComponent(lua_State* L)
	{
		lua_getfield(L, -1, "EntityId");

		// Gets Script Entity
		uint32_t* entityId = (uint32_t*)lua_touserdata(L, -1);
		Entity entity((entt::entity)*entityId, ScriptingEngine::GetSceneContext());

		// Gets String 
		std::string componentType = lua_tostring(L, -3);

		if (componentType == "Transform") {
			TransformComponent& component = entity.GetComponent<TransformComponent>();
			return 1;
		}
		else if (componentType == "MeshRenderer")
		{
			MeshRendererComponent& component = entity.GetComponent<MeshRendererComponent>();
			return 1;
		}

		return 1;
	}

	int ScriptingEngine::HasComponent(lua_State* L)
	{
		// Get Entity Id Struct
		lua_getfield(L, -1, "EntityId");

		// Get Entity
		ScriptEntityID* scriptEntityID = (ScriptEntityID*)lua_touserdata(L, -1);
		Entity entity((entt::entity)scriptEntityID->entityId, ScriptingEngine::GetSceneContext());

		// Get Component Type
		std::string componentType = lua_tostring(L, -3);

		bool hasComponent = false;
		if (componentType == "Transform")
		{
			hasComponent = entity.HasComponent<TransformComponent>();
		}
		else if (componentType == "MeshRenderer") {
			hasComponent = entity.HasComponent<MeshRendererComponent>();
		}

		lua_pushboolean(L, hasComponent);
		return 1;
	}

	int ScriptingEngine::SetComponent(lua_State* L)
	{
		return 0;
	}

	ScriptingEngine* ScriptingEngine::GetInstance()
	{
		if (!s_Instance)
			s_Instance = new ScriptingEngine();

		return s_Instance;
	}

	void ScriptingEngine::Init()
	{
		GetInstance()->InitImpl();
	}

	void ScriptingEngine::InitImpl()
	{
	}

	void ScriptingEngine::Shutdown()
	{	
	}

	lua_State* ScriptingEngine::GetLuaState()
	{
		return GetInstance()->GetLuaStateImpl();
	}

	lua_State* ScriptingEngine::GetLuaStateImpl()
	{
		return m_LuaState;
	}

	void ScriptingEngine::SetSceneContextImpl(Scene* scene)
	{
		m_SceneContext = scene;
	}

	Scene* ScriptingEngine::GetSceneContextImpl()
	{
		return m_SceneContext;
	}
}
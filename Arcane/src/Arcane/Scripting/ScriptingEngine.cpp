#include <iostream>

#include "ScriptingEngine.h"
#include "Arcane/Core/InputManager.h"
#include "Arcane/ECS/Entity.h"

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


	static int l_InputManager_GetKeyReleased(lua_State* L) {
		// Get Key Code
		int keyCode = lua_tonumber(L, 1);

		// Check Key Code
		bool isReleased = InputManager::GetKeyReleased(keyCode);

		// Push Boolean onto stack
		lua_pushboolean(L, isReleased);
		
		return 1;
	}

	static int l_InputManager_GetKeyPressed(lua_State* L) {
		// Get Key Code
		int keyCode = lua_tonumber(L, 1);

		// Check Key Code
		bool isPressed = InputManager::GetKeyPressed(keyCode);

		// Push Boolean onto stack
		lua_pushboolean(L, isPressed);
		return 1;
	}

	static int l_GetComponent(lua_State* L) {
		lua_getfield(L, -1, "EntityId");

		// Gets Script Entity
		ScriptEntityID* scriptEntityID = (ScriptEntityID*)lua_touserdata(L, -1);
		Entity entity((entt::entity)scriptEntityID->entityId, scriptEntityID->scene);

		// Gets String 
		std::string componentType = lua_tostring(L, -3);

		if (componentType == "Transform") {
			TransformComponent& component = entity.GetComponent<TransformComponent>();
		}

		return 1;
	}

	static int l_HasComponent(lua_State* L)
	{
		// Get Entity Id Struct
		lua_getfield(L, -1, "EntityId");
		
		// Get Entity
		ScriptEntityID* scriptEntityID = (ScriptEntityID*)lua_touserdata(L, -1);
		Entity entity((entt::entity)scriptEntityID->entityId, scriptEntityID->scene);
		
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

	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		m_LuaState = luaL_newstate();
		luaL_openlibs(m_LuaState);
		
		// Register Functions
		lua_pushcfunction(m_LuaState, l_InputManager_GetKeyReleased);
		lua_setglobal(m_LuaState, "GetKeyReleased");

		lua_pushcfunction(m_LuaState, l_InputManager_GetKeyPressed);
		lua_setglobal(m_LuaState, "GetKeyPressed");

		lua_pushcfunction(m_LuaState, l_GetComponent);
		lua_setglobal(m_LuaState, "GetComponent");

		lua_pushcfunction(m_LuaState, l_HasComponent);
		lua_setglobal(m_LuaState, "HasComponent");
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
}
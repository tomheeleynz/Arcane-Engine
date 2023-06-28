#include <iostream>

#include "ScriptingEngine.h"
#include "Arcane/Core/InputManager.h"

namespace Arcane
{
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
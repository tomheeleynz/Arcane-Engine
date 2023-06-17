#include <iostream>

#include "ScriptingEngine.h"

namespace Arcane
{

	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		m_LuaState = luaL_newstate();
		luaL_openlibs(m_LuaState);
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
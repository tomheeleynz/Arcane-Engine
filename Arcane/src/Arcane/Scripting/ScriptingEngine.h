#pragma once

#include <string>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}


namespace Arcane
{
	class ScriptingEngine
	{
	public:
		static ScriptingEngine* GetInstance();
		static void Shutdown();
		static void Init();
		static lua_State* GetLuaState();
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();

		lua_State* GetLuaStateImpl();
	private:
		lua_State* m_LuaState;
	};
}
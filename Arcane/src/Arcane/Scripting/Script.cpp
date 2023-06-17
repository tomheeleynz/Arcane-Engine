#include "Script.h"
#include "Arcane/Scene/Scene.h"

namespace Arcane
{
	Script::Script(std::string name)
	{
		// Get state of new file
		luaL_dofile(ScriptingEngine::GetLuaState(), name.c_str());
	}

	void Script::OnStart()
	{
		lua_getglobal(ScriptingEngine::GetLuaState(), "OnStart");
		lua_pcall(ScriptingEngine::GetLuaState(), 0, 0, 0);
	}

	void Script::OnUpdate(float deltaTime)
	{
		lua_getglobal(ScriptingEngine::GetLuaState(), "OnUpdate");
		lua_pushnumber(ScriptingEngine::GetLuaState(), deltaTime);
		lua_pcall(ScriptingEngine::GetLuaState(), 1, 0, 0);
	}
}
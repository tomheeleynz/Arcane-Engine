#pragma once
#include <string>
#include <filesystem>

#include "Arcane/Assets/Asset.h"
#include "ScriptingEngine.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace Arcane
{
	class Script : public Asset
	{
	public:
		Script(std::string name);

		void OnStart();
		void OnUpdate(float deltaTime);
	private:
		lua_State* m_LuaState;
	};
}
#pragma once

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace Arcane
{
	class ScriptGlue
	{
	public:
		static void RegisterTables(lua_State* L);
		static void RegisterFunctions(lua_State* L);
		static void RegisterMetatables(lua_State* L);
	private:
		static void CreateVec2Metatable(lua_State* L);
		static void CreateVec3Metatable(lua_State* L);
		static void CreateEntityIdMetatable(lua_State* L);
		static void CreateTransformComponentMetatable(lua_State* L);
	private:
		static void CreateVec2Table(lua_State* L);
		static void CreateVec3Table(lua_State* L);
		static void CreateEntityIdTable(lua_State* L);
		static void CreateTransformComponentTable(lua_State* L);
	};
}
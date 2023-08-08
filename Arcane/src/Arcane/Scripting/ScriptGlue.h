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

		//////////////////////////////////////////////////////////
		///// Entity Functions
		//////////////////////////////////////////////////////////
		static int GetComponent(lua_State* L);
		static int SetComponent(lua_State* L);
	private:

		//////////////////////////////////////////////////////////
		///// Structure Tables
		//////////////////////////////////////////////////////////
		static void CreateVec2Metatable(lua_State* L);
		static void CreateVec3Metatable(lua_State* L);
		static void CreateEntityMetatable(lua_State* L);

		//////////////////////////////////////////////////////////
		///// Component Tables
		//////////////////////////////////////////////////////////
		static void CreateTransformComponentMetatable(lua_State* L);
		static void CreateSpriteRendererComponentMetatable(lua_State* L);
	};
}
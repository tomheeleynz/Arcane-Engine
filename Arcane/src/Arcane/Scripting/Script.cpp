#include "Script.h"
#include "Arcane/Scene/Scene.h"

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


	Script::Script(std::string name)
	{
		// Places the entity table on the lua stack
		luaL_dofile(ScriptingEngine::GetLuaState(), name.c_str());

		// Load Properies
		LoadProperties();

		// Store Start Function in lua registery
		lua_pop(ScriptingEngine::GetLuaState(), 1);
		
		// Get start function
		lua_getfield(ScriptingEngine::GetLuaState(), -1, "OnStart");
		PrintStack(ScriptingEngine::GetLuaState());
		m_StartIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);
		
		// Get Update Function
		lua_getfield(ScriptingEngine::GetLuaState(), -1, "OnUpdate");
		PrintStack(ScriptingEngine::GetLuaState());
		m_UpdateIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);
		PrintStack(ScriptingEngine::GetLuaState());

		// Push an object reference onto the registery table (for later use)
		lua_pushvalue(ScriptingEngine::GetLuaState(), -1);
		PrintStack(ScriptingEngine::GetLuaState());
		m_ObjectIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);

		// Clear Stack Ready for next script
		lua_settop(ScriptingEngine::GetLuaState(), 0);
	}

	void Script::OnStart()
	{
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_StartIndex);
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_ObjectIndex);
		lua_pcall(ScriptingEngine::GetLuaState(), 1, 0, 0);
	}

	void Script::OnUpdate(float deltaTime)
	{		
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_UpdateIndex);
		
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_ObjectIndex);
		lua_pushnumber(ScriptingEngine::GetLuaState(), deltaTime);
		
		lua_pcall(ScriptingEngine::GetLuaState(), 2, 0, 0);
	}

	void Script::SetEntityID(uint64_t entityID)
	{
	}

	void Script::LoadScriptProperites()
	{
	}

	void Script::LoadProperties()
	{
		lua_pushstring(ScriptingEngine::GetLuaState(), "Properties");
		lua_gettable(ScriptingEngine::GetLuaState(), -2);
	
		lua_pushnil(ScriptingEngine::GetLuaState());
		while (lua_next(ScriptingEngine::GetLuaState(), -2))
		{
			lua_pushvalue(ScriptingEngine::GetLuaState(), -2);
			
			const char* key = lua_tostring(ScriptingEngine::GetLuaState(), -1);
			
			std::cout << key << std::endl;

			ScriptProperty newProperty;

			if (lua_isinteger(ScriptingEngine::GetLuaState(), -2)) 
			{
				newProperty.type = "int";
				newProperty.value = (int)lua_tonumber(ScriptingEngine::GetLuaState(), -2);
			}

			if (lua_isnumber(ScriptingEngine::GetLuaState(), -2) && !lua_isinteger(ScriptingEngine::GetLuaState(), -2))
			{
				newProperty.type = "float";
				newProperty.value = (float)lua_tonumber(ScriptingEngine::GetLuaState(), -2);
			}

			if (lua_isstring(ScriptingEngine::GetLuaState(), -2) && !lua_isnumber(ScriptingEngine::GetLuaState(), -2))
			{
				newProperty.type = "string";
				newProperty.value = (std::string)lua_tostring(ScriptingEngine::GetLuaState(), -2);
			}

			m_Properties[key] = newProperty;
			lua_pop(ScriptingEngine::GetLuaState(), 2);
		}
	}
}
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
	}

	void Script::OnStart()
	{
	}

	void Script::OnUpdate(float deltaTime)
	{		
	}

	void Script::SetEntityID(uint64_t entityID)
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

			//const char* value = lua_tostring(L, -2);
			//printf("%s => %s\n", key, value);
			lua_pop(ScriptingEngine::GetLuaState(), 2);
		}
	}
}
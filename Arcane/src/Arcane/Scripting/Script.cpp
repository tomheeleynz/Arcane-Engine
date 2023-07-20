#include "Script.h"
#include "Arcane/Scene/Scene.h"
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

	Script::Script(std::string name)
	{
		lua_settop(ScriptingEngine::GetLuaState(), 0);
		// Places the entity table on the lua stack
		luaL_dofile(ScriptingEngine::GetLuaState(), name.c_str());

		m_ObjectIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);

		// Clear Stack Ready for next script
		lua_settop(ScriptingEngine::GetLuaState(), 0);
	}

	void Script::OnStart()
	{
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_StartIndex);
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_EntityIdIndex);
		lua_pcall(ScriptingEngine::GetLuaState(), 1, 0, 0);
	}

	void Script::OnUpdate(float deltaTime)
	{		
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_UpdateIndex);
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_EntityIdIndex);
		lua_pushnumber(ScriptingEngine::GetLuaState(), deltaTime);
		lua_pcall(ScriptingEngine::GetLuaState(), 2, 0, 0);
	}

	void Script::SetEntity(Entity& entity)
	{
		lua_State* L = ScriptingEngine::GetLuaState();
		lua_newtable(L);
		int entityTableIdx = lua_gettop(L);
		PrintStack(L);

		Entity* entityPtr = (Entity*)lua_newuserdata(L, sizeof(Entity));
		*entityPtr = entity;
		PrintStack(L);

		lua_setfield(L, -2, "EntityId");
		PrintStack(L);

		lua_pushcfunction(L, ScriptingEngine::GetComponent);
		PrintStack(L);

		lua_setfield(L, -2, "GetComponent");
		PrintStack(L);

		lua_pushcfunction(L, ScriptingEngine::HasComponent);
		PrintStack(L);

		lua_setfield(L, -2, "HasComponent");
		PrintStack(L);

		lua_pushcfunction(L, ScriptingEngine::SetTransform);
		PrintStack(L);

		lua_setfield(L, -2, "SetTranslation");
		PrintStack(L);


		lua_rawgeti(L, LUA_REGISTRYINDEX, m_ObjectIndex); 
		PrintStack(L);

		// Copy Across Properties
		lua_getfield(L, -1, "Properties");
		PrintStack(L);

		lua_setfield(L, -3, "Properties");
		PrintStack(L);

		// Set Metatable
		lua_setmetatable(L, -2);
		PrintStack(L);

		// Get OnStart and Update metamethods
		luaL_getmetafield(L, -1, "OnStart");
		PrintStack(L);
		m_StartIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);
		PrintStack(L);

		luaL_getmetafield(L, -1, "OnUpdate");
		PrintStack(L);
		m_UpdateIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);
		PrintStack(L);

		m_EntityIdIndex = luaL_ref(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX);

		lua_settop(L, 0);
		LoadProperties();
	}

	void Script::LoadScriptProperites()
	{
		// Clear lua stack
		lua_settop(ScriptingEngine::GetLuaState(), 0);

		// need to push object onto stack
		lua_rawgeti(ScriptingEngine::GetLuaState(), LUA_REGISTRYINDEX, m_EntityIdIndex);

		// Get properties
		lua_pushstring(ScriptingEngine::GetLuaState(), "Properties");
		lua_gettable(ScriptingEngine::GetLuaState(), -2);

		// iterate through fields and set them 
		lua_pushnil(ScriptingEngine::GetLuaState());
	
		while (lua_next(ScriptingEngine::GetLuaState(), -2))
		{
			// Get Key and Value
			lua_pushvalue(ScriptingEngine::GetLuaState(), -2);

			const char* key = lua_tostring(ScriptingEngine::GetLuaState(), -1);

			std::string type = m_Properties[key].type;
			std::any value = m_Properties[key].value;

			if (type == "int") {
				// Working
				int intValue = std::any_cast<int>(value);
				lua_pushinteger(ScriptingEngine::GetLuaState(), intValue);
				lua_setfield(ScriptingEngine::GetLuaState(), -5, key);
			}

			if (type == "float") {
				// Working
				float floatValue = std::any_cast<float>(value);
				lua_pushnumber(ScriptingEngine::GetLuaState(), floatValue);
				lua_setfield(ScriptingEngine::GetLuaState(), -5, key);
			}

			if (type == "string") {
				// Working
				std::string stringValue = std::any_cast<std::string>(value);
				lua_pushstring(ScriptingEngine::GetLuaState(), stringValue.c_str());
				lua_setfield(ScriptingEngine::GetLuaState(), -5, key);
			}

			if (type == "Vector3") {
				// Working
				glm::vec3 vector3Value = std::any_cast<glm::vec3>(value);
				glm::vec3* ptrToUserData = (glm::vec3*)lua_touserdata(ScriptingEngine::GetLuaState(), -2);
				ptrToUserData->x = vector3Value.x;
				ptrToUserData->y = vector3Value.y;
				ptrToUserData->z = vector3Value.z;
			} 
			if (type == "Vector2") {
				// Working
				glm::vec2 vector2Value = std::any_cast<glm::vec2>(value);
				glm::vec2* ptrToUserData = (glm::vec2*)lua_touserdata(ScriptingEngine::GetLuaState(), -2);
				ptrToUserData->x = vector2Value.x;
				ptrToUserData->y = vector2Value.y;
			}

			lua_pop(ScriptingEngine::GetLuaState(), 2);
		}
	}

	void Script::LoadProperties()
	{
		lua_State* L = ScriptingEngine::GetLuaState();
		PrintStack(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_EntityIdIndex);
		PrintStack(L);

		lua_getfield(L, -1, "Properties");
		PrintStack(L);
	
		lua_pushnil(L);
		PrintStack(L);
		while (lua_next(L, -2))
		{
			PrintStack(L);
			const char* key = lua_tostring(L, -2);
			ScriptProperty newProperty;

			if (lua_isinteger(ScriptingEngine::GetLuaState(), -1)) 
			{
				newProperty.type = "int";
				newProperty.value = (int)lua_tonumber(ScriptingEngine::GetLuaState(), -2);
			}

			if (lua_isnumber(ScriptingEngine::GetLuaState(), -1) && !lua_isinteger(ScriptingEngine::GetLuaState(), -1))
			{
				newProperty.type = "float";
				newProperty.value = (float)lua_tonumber(ScriptingEngine::GetLuaState(), -2);
			}

			if (lua_isstring(ScriptingEngine::GetLuaState(), -1) && !lua_isnumber(ScriptingEngine::GetLuaState(), -1))
			{
				newProperty.type = "string";
				newProperty.value = (std::string)lua_tostring(ScriptingEngine::GetLuaState(), -2);
			} 

			if (lua_istable(L, -1))
			{
				newProperty.type = "Entity";
				newProperty.value = nullptr;

				// Add functions to this to load it up

			}

			if (lua_isuserdata(ScriptingEngine::GetLuaState(), -1)) 
			{
				if (luaL_testudata(ScriptingEngine::GetLuaState(), -1, "Vector3Metatable") != nullptr) {
					newProperty.type = "Vector3";
					newProperty.value = *(glm::vec3*)lua_touserdata(ScriptingEngine::GetLuaState(), -1);
				}

				if (luaL_testudata(ScriptingEngine::GetLuaState(), -1, "Vector2Metatable") != nullptr) {
					newProperty.type = "Vector2";
					newProperty.value = *(glm::vec2*)lua_touserdata(ScriptingEngine::GetLuaState(), -1);
				}
			}

			m_Properties[key] = newProperty;
			lua_pop(ScriptingEngine::GetLuaState(), 1);
		}
	}
}
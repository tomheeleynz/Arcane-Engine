#include "ScriptGlue.h"

#include "Arcane/Scripting/ScriptingEngine.h"
#include "Arcane/ECS/Component.h"
#include "Arcane/Core/InputManager.h"
#include "Script.h"


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

	void ScriptGlue::RegisterTables(lua_State* L)
	{
		CreateVec2Table(L);
		CreateVec3Table(L);
		CreateTransformComponentTable(L);
	}

	void ScriptGlue::RegisterFunctions(lua_State* L)
	{
		// Global Functions
		lua_pushcfunction(L, l_InputManager_GetKeyReleased);
		lua_setglobal(L, "GetKeyReleased");

		lua_pushcfunction(L, l_InputManager_GetKeyPressed);
		lua_setglobal(L, "GetKeyPressed");
	}

	void ScriptGlue::RegisterMetatables(lua_State* L)
	{
		CreateVec2Metatable(L);
		CreateVec3Metatable(L);
		CreateTransformComponentMetatable(L);
	}

	void ScriptGlue::CreateVec2Metatable(lua_State* L)
	{
		luaL_newmetatable(L, "Vector2Metatable");

		// Index the field
		auto Vector2Index = [](lua_State* L) -> int {
			glm::vec2* vec2 = (glm::vec2*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);

			if (strcmp(index, "x") == 0) {
				lua_pushnumber(L, vec2->x);
				return 1;
			}
			else if (strcmp(index, "y") == 0) {
				lua_pushnumber(L, vec2->y);
				return 1;
			}
			else {
				lua_getglobal(L, "Vector2");
				lua_pushstring(L, index);
				lua_rawget(L, -2);
				return 1;
			}
		};

		// New Index field
		auto Vector2NewIndex = [](lua_State* L) -> int {
			glm::vec2* vec2 = (glm::vec2*)lua_touserdata(L, -3);
			
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "x") == 0)
			{
				vec2->x = (int)lua_tonumber(L, -1);
			}
			else if (strcmp(index, "y") == 0)
			{
				vec2->y = (int)lua_tonumber(L, -1);
			}
			else
			{
				assert(false);	
			}

			return 0;
		};

		auto Vec2ToString = [](lua_State* L) -> int {
			glm::vec2* vec2 = (glm::vec2*)lua_touserdata(L, -3);

			std::string returnString = "X: " + std::to_string(vec2->x) + ", Y: " + std::to_string(vec2->y);
			lua_pushstring(L, returnString.c_str());
			return 1;
		};

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, Vector2Index);
		lua_settable(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, Vector2NewIndex);
		lua_settable(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, Vec2ToString);
		lua_settable(L, -3);
	}

	void ScriptGlue::CreateVec3Metatable(lua_State* L)
	{
		luaL_newmetatable(L, "Vector3Metatable");

		// Index the field
		auto Vector3Index = [](lua_State* L) -> int {
			glm::vec3* vec3 = (glm::vec3*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);

			if (strcmp(index, "x") == 0) {
				lua_pushnumber(L, vec3->x);
				return 1;
			}
			else if (strcmp(index, "y") == 0) {
				lua_pushnumber(L, vec3->y);
				return 1;
			}
			else if (strcmp(index, "z") == 0) {
				lua_pushnumber(L, vec3->z);
				return 1;
			}
			else {
				lua_getglobal(L, "Vector3");
				lua_pushstring(L, index);
				lua_rawget(L, -2);
				return 1;
			}
		};

		// New Index field
		auto Vector3NewIndex = [](lua_State* L) -> int {
			glm::vec3* vec3 = (glm::vec3*)lua_touserdata(L, -3);

			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "x") == 0)
			{
				vec3->x = (int)lua_tonumber(L, -1);
			}
			else if (strcmp(index, "y") == 0)
			{
				vec3->y = (int)lua_tonumber(L, -1);
			}
			else if (strcmp(index, "z") == 0) 
			{
				vec3->z = (int)lua_tonumber(L, -1);
			}
			else
			{
				assert(false);
			}

			return 0;
		};

		auto Vec3ToString = [](lua_State* L) -> int {
			glm::vec3* vec3 = (glm::vec3*)lua_touserdata(L, -3);

			std::string returnString = "X: " + std::to_string(vec3->x) + ", Y: " + std::to_string(vec3->y) + ", Z: " + std::to_string(vec3->z);
			lua_pushstring(L, returnString.c_str());
			return 1;
		};

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, Vector3Index);
		lua_settable(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, Vector3NewIndex);
		lua_settable(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, Vec3ToString);
		lua_settable(L, -3);
	}

	void ScriptGlue::CreateTransformComponentMetatable(lua_State* L)
	{
		luaL_newmetatable(L, "TransformComponentMetatable");

		auto TransformComponentIndex = [](lua_State* L) -> int {
			TransformComponent** transformComponentPtr = (TransformComponent**)lua_touserdata(L, -2);
			TransformComponent* transformComponent = *transformComponentPtr;
			const char* index = lua_tostring(L, -1);

			if (strcmp(index, "translation") == 0) {
				// Create Transform Vector
				glm::vec3* translation = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
				translation->x = transformComponent->pos.x;
				translation->y = transformComponent->pos.y;
				translation->z = transformComponent->pos.z;

				luaL_getmetatable(L, "Vector3Metatable");
				lua_setmetatable(L, -2);
				return 1;
			}
			else {
				lua_getglobal(L, "TransformComponent");
				lua_pushstring(L, index);
				lua_rawget(L, -2);
				return 1;
			}
		};

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, TransformComponentIndex);
		lua_settable(L, -3);
	}

	void ScriptGlue::CreateTransformComponentTable(lua_State* L)
	{
		lua_newtable(L);
		int transformComponentTable = lua_gettop(L);
		lua_pushvalue(L, transformComponentTable);
		lua_setglobal(L, "TransformComponent");

		auto CreateTransformComponent = [](lua_State* L) -> int {
			void* ptrToTransformComponent = lua_newuserdata(L, sizeof(TransformComponent));
			new (ptrToTransformComponent) TransformComponent();
			luaL_getmetatable(L, "TransformComponentMetatable");
			lua_setmetatable(L, -2);
			return 1;
		};

		lua_pushcfunction(L, CreateTransformComponent);
		lua_setfield(L, -2, "new");
	}

	void ScriptGlue::CreateVec2Table(lua_State* L)
	{
		lua_newtable(L);
		int vec2TableIdx = lua_gettop(L);
		lua_pushvalue(L, vec2TableIdx);
		lua_setglobal(L, "Vector2");

		// Functions
		auto CreateVector2 = [](lua_State* L) -> int {
			void* ptrToVec2 = lua_newuserdata(L, sizeof(glm::vec2));
			new (ptrToVec2) glm::vec2();
			luaL_getmetatable(L, "Vector2Metatable");
			lua_setmetatable(L, -2);
			return 1;
		};

		lua_pushcfunction(L, CreateVector2);
		lua_setfield(L, -2, "new");
	}

	void ScriptGlue::CreateVec3Table(lua_State* L)
	{
		lua_newtable(L);
		int vec3TableIdx = lua_gettop(L);
		lua_pushvalue(L, vec3TableIdx);
		lua_setglobal(L, "Vector3");

		// Functions
		auto CreateVector3 = [](lua_State* L) -> int {
			void* ptrToVec3 = lua_newuserdata(L, sizeof(glm::vec3));
			new (ptrToVec3) glm::vec3();
			luaL_getmetatable(L, "Vector3Metatable");
			lua_setmetatable(L, -2);
			return 1;
		};

		lua_pushcfunction(L, CreateVector3);
		lua_setfield(L, -2, "new");
	}


}
#include "ScriptGlue.h"

#include "Arcane/Scripting/ScriptingEngine.h"
#include "Arcane/ECS/Component.h"
#include "Arcane/Core/InputManager.h"
#include "Script.h"
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

	///////////////////////////////////////////////////////////
	//// Global Functions
	///////////////////////////////////////////////////////////
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

	/////////////////////////////////////////////////////////////////////
	//// Creation of Structures Functions
	/////////////////////////////////////////////////////////////////////
	static int l_Vector3_Create(lua_State* L)
	{
		glm::vec3* newVec3 = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
		newVec3->x = 1;
		newVec3->y = 0;
		newVec3->z = 0;
		luaL_getmetatable(L, "Vector3Metatable");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int l_Vector2_Create(lua_State* L)
	{
		glm::vec2* newVec2 = (glm::vec2*)lua_newuserdata(L, sizeof(glm::vec2));
		newVec2->x = 2;
		newVec2->y = 0;
		luaL_getmetatable(L, "Vector2Metatable");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int l_SpriteRendererCreate(lua_State* L)
	{
		PrintStack(L);
		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)lua_newuserdata(L, sizeof(SpriteRendererComponent));
		spriteRenderer->color = { 1.0f, 1.0f, 1.0f };
		spriteRenderer->sprite = nullptr;
		spriteRenderer->flipX = false;
		spriteRenderer->flipY = false;
		PrintStack(L);

		luaL_getmetatable(L, "SpriteRendererComponentMetatable");
		PrintStack(L);
		lua_setmetatable(L, -2);
		return 1;
	}

	/////////////////////////////////////////////////////////////////////
	//// ECS Interfacing
	/////////////////////////////////////////////////////////////////////
	int ScriptGlue::GetComponent(lua_State* L)
	{
		lua_getfield(L, -2, "EntityId");
		Entity* entity = (Entity*)lua_touserdata(L, -1);
		const char* componentType = lua_tostring(L, -2);

		if (strcmp(componentType, "Transform") == 0)
		{
			// This is now on top of stack
			TransformComponent* newTransformComponent = (TransformComponent*)lua_newuserdata(L, sizeof(TransformComponent));
			luaL_setmetatable(L, "TransformComponentMetatable");
			*newTransformComponent = entity->GetComponent<TransformComponent>();

			return 1;
		}

		if (strcmp(componentType, "SpriteRenderer") == 0)
		{
			SpriteRendererComponent* newSpriteRendererComponent = (SpriteRendererComponent*)lua_newuserdata(L, sizeof(SpriteRendererComponent));
			luaL_setmetatable(L, "SpriteRendererComponentMetatable");
			*newSpriteRendererComponent = entity->GetComponent<SpriteRendererComponent>();
			return 1;
		}

		return 1;
	}

	int ScriptGlue::SetComponent(lua_State* L)
	{
		lua_getfield(L, -3, "EntityId");
		Entity* entity = (Entity*)lua_touserdata(L, -1);
		const char* componentType = lua_tostring(L, -3);
		PrintStack(L);

		if (strcmp(componentType, "SpriteRenderer") == 0)
		{
			SpriteRendererComponent* spriteRendererComponent = (SpriteRendererComponent*)lua_touserdata(L, -2);
			entity->GetComponent<SpriteRendererComponent>() = *spriteRendererComponent;
		}

		return 1;

	}

	int ScriptGlue::ScriptWriter(lua_State* L, const void* p, size_t sz, void* ud)
	{
		LuaByteCode* bd = (LuaByteCode*)ud;
		char* newData = (char*)realloc(*(bd->data), (*(bd->len)) + sz);

		if (newData)
		{
			memcpy(newData + (*(bd->len)), p, sz);
			*(bd->data) = newData;
			*(bd->len) += sz;
		}
		else {
			free(newData);
			return 1;
		}

		return 0;
	}

	void ScriptGlue::RegisterTables(lua_State* L)
	{
	}

	void ScriptGlue::RegisterFunctions(lua_State* L)
	{
		// Global Functions
		lua_pushcfunction(L, l_InputManager_GetKeyReleased);
		lua_setglobal(L, "GetKeyReleased");

		lua_pushcfunction(L, l_InputManager_GetKeyPressed);
		lua_setglobal(L, "GetKeyPressed");

		// Structure Components
		lua_pushcfunction(L, l_Vector3_Create);
		lua_setglobal(L, "Vector3");

		lua_pushcfunction(L, l_Vector2_Create);
		lua_setglobal(L, "Vector2");

		lua_pushcfunction(L, l_SpriteRendererCreate);
		lua_setglobal(L, "SpriteRenderer");

		// ECS Functions
	}

	void ScriptGlue::RegisterMetatables(lua_State* L)
	{
		CreateVec2Metatable(L);
		CreateVec3Metatable(L);
		CreateTransformComponentMetatable(L);
		CreateSpriteRendererComponentMetatable(L);
		CreateEntityMetatable(L);
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
				vec3->x = (float)lua_tonumber(L, -1);
			}
			else if (strcmp(index, "y") == 0)
			{
				vec3->y = (float)lua_tonumber(L, -1);
			}
			else if (strcmp(index, "z") == 0) 
			{
				vec3->z = (float)lua_tonumber(L, -1);
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

	void ScriptGlue::CreateEntityMetatable(lua_State* L)
	{
		luaL_newmetatable(L, "EntityMetatable");

		auto EntityIndex = [](lua_State* L) -> int {
			lua_getfield(L, -2, "EntityId");
			Entity* entity = (Entity*)lua_touserdata(L, -1);
			return 1;
		};

		lua_pushcfunction(L, EntityIndex);
		lua_setfield(L, -2, "__index");
	}

	void ScriptGlue::CreateTransformComponentMetatable(lua_State* L)
	{
		luaL_newmetatable(L, "TransformComponentMetatable");

		auto TransformComponentIndex = [](lua_State* L) -> int {
			PrintStack(L);
			TransformComponent* transformComponentPtr = (TransformComponent*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);

			if (strcmp(index, "translation") == 0) {
				// Create Transform Vector
				glm::vec3* translation = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
				translation->x = transformComponentPtr->pos.x;
				translation->y = transformComponentPtr->pos.y;
				translation->z = transformComponentPtr->pos.z;

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

	void ScriptGlue::CreateSpriteRendererComponentMetatable(lua_State* L)
	{
		luaL_newmetatable(L, "SpriteRendererComponentMetatable");

		auto SpriteRendererComponentIndex = [](lua_State* L) -> int {
			SpriteRendererComponent* spriteRendererComponent = (SpriteRendererComponent*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);

			if (strcmp(index, "color") == 0)
			{
				glm::vec3* color = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
				color->x = spriteRendererComponent->color.r;
				color->y = spriteRendererComponent->color.g;
				color->z = spriteRendererComponent->color.b;
				
				luaL_getmetatable(L, "Vector3Metatable");
				lua_setmetatable(L, -2);
				return 1;
			}
			return 1;
		};

		auto SpriteRendererComponentNewIndex = [](lua_State* L) -> int {
			const char* componentType = lua_tostring(L, -2);

			if (strcmp(componentType, "color") == 0)
			{
				SpriteRendererComponent* spriteRendererComponent = (SpriteRendererComponent*)lua_touserdata(L, -3);
				glm::vec3* colorVec = (glm::vec3*)lua_touserdata(L, -1);

				spriteRendererComponent->color = *colorVec;
			}
			return 0;
		};

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, SpriteRendererComponentIndex);
		lua_settable(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, SpriteRendererComponentNewIndex);
		lua_settable(L, -3);
	}

}
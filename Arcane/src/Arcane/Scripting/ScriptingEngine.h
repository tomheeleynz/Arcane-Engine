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
	class Scene;

	class ScriptingEngine
	{
	public:
		static ScriptingEngine* GetInstance();
		static void Shutdown();
		static void Init();
		static lua_State* GetLuaState();

		// Set Scene Context
		static void SetSceneContext(Scene* scene);
		static Scene* GetSceneContext();

		// Functions that require access to the scene context
		static int GetComponent(lua_State* L);
		static int HasComponent(lua_State* L);
		static int SetComponent(lua_State* L);
		static int SetTransform(lua_State* L);
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();

		lua_State* GetLuaStateImpl();

		// Scene Context
		void SetSceneContextImpl(Scene* scene);
		Scene* GetSceneContextImpl();
	private:
		lua_State* m_LuaState;
		Scene* m_SceneContext;
	};
}
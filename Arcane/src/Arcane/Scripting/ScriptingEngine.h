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
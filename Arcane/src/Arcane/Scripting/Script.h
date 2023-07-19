#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <any>

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
	class Entity;
	class Scene;

	struct ScriptProperty
	{
		std::string type;
		std::any value;
	};

	struct ScriptEntityID
	{
		ScriptEntityID(Core::UUID* entityUUID) { uuid = entityUUID; }
		Core::UUID* uuid;
	};

	class Script : public Asset
	{
	public:
		Script(std::string name);

		void OnStart();
		void OnUpdate(float deltaTime);

		std::unordered_map<std::string, ScriptProperty> GetProperties() { return m_Properties; }
		void SetPropertyValue(std::string key, std::any newValue) { m_Properties[key].value = newValue; }

		void LoadScriptProperites();
		void SetEntity(Entity& entity);
	private:
		void LoadProperties();
	private:
		lua_State* m_LuaState;
		std::unordered_map<std::string, ScriptProperty> m_Properties;

		int m_StartIndex = 0;
		int m_UpdateIndex = 0;
		int m_ObjectIndex = 0;
		int m_EntityIdIndex = 0;
	};
}
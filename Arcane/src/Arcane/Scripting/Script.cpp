#include "Script.h"
#include "Arcane/Scene/Scene.h"
#include "Arcane/Scripting/PythonBindings.h"

namespace Arcane
{
	Script::Script(std::string name)
	{
		m_Module = py::module::import(name.c_str());
		
		m_Class = m_Module.attr(name.c_str());
		m_Object = m_Class.call();
	}

	void Script::OnStart()
	{
		m_Object.attr("OnStart").call();
	}

	void Script::OnUpdate(float deltaTime)
	{
		m_Object.attr("OnUpdate").call(deltaTime);
	}

	uint32_t Script::GetEntityID()
	{
		return m_EntityID;
	}

	void Script::SetEntityID(uint32_t id)
	{
		m_Object.attr("id") = m_EntityID;
		m_EntityID = id;
	}

	void Script::SetScriptScene(Scene* scene)
	{
		m_ScriptScene = scene;
	}
	
	Scene* Script::GetScene()
	{
		return m_ScriptScene;
	}
}
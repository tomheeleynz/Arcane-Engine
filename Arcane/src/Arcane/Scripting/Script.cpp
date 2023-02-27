#include "Script.h"

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
}
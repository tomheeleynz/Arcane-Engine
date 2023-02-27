#include "Script.h"

namespace Arcane
{
	Script::Script(std::string name)
	{
		m_Module = py::module::import(name.c_str());
	}

	void Script::OnStart()
	{
		m_Module.attr("OnStart").call();
	}

	void Script::OnUpdate()
	{
		m_Module.attr("OnUpdate").call();
	}
}
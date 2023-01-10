#include "Script.h"

namespace Arcane
{
	Script::Script(std::filesystem::path path)
	{
		std::string pathDir = path.parent_path().string();
		std::string scriptName = path.stem().string();

		std::wstring widestr = std::wstring(pathDir.begin(), pathDir.end());
		PySys_SetPath(widestr.c_str());
		
		// Get Module
		PyObject* pName = PyUnicode_FromString(scriptName.c_str());
		m_Module = PyImport_Import(pName);

		// Get Start Func
		m_StartFunction = PyObject_GetAttrString(m_Module, "Start");

		// Get Update Func
		m_UpdateFunction = PyObject_GetAttrString(m_Module, "Update");
	}
}
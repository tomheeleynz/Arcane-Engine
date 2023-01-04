#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

#include "ScriptingEngine.h"

namespace Arcane
{
	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		Py_Initialize();
	}

	ScriptingEngine* ScriptingEngine::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new ScriptingEngine();

		return s_Instance;
	}

	void ScriptingEngine::Init()
	{
		GetInstance()->InitImpl();
	}

	void ScriptingEngine::InitImpl()
	{
	}

	PyObject* ScriptingEngine::GetModule(std::string modulePath)
	{
		return GetInstance()->GetModuleImpl(modulePath);
	}

	PyObject* ScriptingEngine::GetModuleImpl(std::string modulePath)
	{
		std::wstring widestr = std::wstring(modulePath.begin(), modulePath.end());
		PySys_SetPath(widestr.c_str());
		PyObject* pName = PyUnicode_FromString("test");
		PyObject* module = PyImport_Import(pName);
		Py_DECREF(pName);
		return module;
	}

	PyObject* ScriptingEngine::GetFunc(const char* funcName, PyObject* module)
	{
		return GetInstance()->GetFuncImpl(funcName, module);
	}


	PyObject* ScriptingEngine::GetFuncImpl(const char* funcName, PyObject* module)
	{
		PyObject* func = PyObject_GetAttrString(module, funcName);
		return func;
	}

	void ScriptingEngine::CallFunc(PyObject* func)
	{
		GetInstance()->CallFuncImpl(func);
	}

	void ScriptingEngine::CallFuncImpl(PyObject* func)
	{
		PyObject_CallObject(func, nullptr);
	}
}
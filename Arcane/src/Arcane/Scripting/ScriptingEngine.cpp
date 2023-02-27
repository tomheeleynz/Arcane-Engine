#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <pybind11/embed.h>
#include <iostream>

#include "ScriptingEngine.h"

#include "Arcane/Core/InputManager.h"

namespace py = pybind11;

namespace Arcane
{
	PYBIND11_EMBEDDED_MODULE(ArcanePythonModule, m) {
		py::class_<InputManager>(m, "InputManager")
			.def("GetKeyPressed", &InputManager::GetKeyPressed)
			.def("GetKeyReleased", &InputManager::GetKeyReleased);
	}

	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		py::initialize_interpreter();
		
		py::print("Hello from python interp");
		auto arcanePythonModule = py::module::import("ArcanePythonModule");

		// Append Working Dir to path
		py::module_ sys = py::module_::import("sys");
		sys.attr("path").attr("append")("C:\\Projects\\BasicGame\\Scripts");
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

	void ScriptingEngine::CallFunc(PyObject* func)
	{
		GetInstance()->CallFuncImpl(func);
	}

	void ScriptingEngine::CallFuncImpl(PyObject* func)
	{
		PyObject_CallObject(func, nullptr);
	}

	void ScriptingEngine::Shutdown()
	{
		py::finalize_interpreter();
	}
}
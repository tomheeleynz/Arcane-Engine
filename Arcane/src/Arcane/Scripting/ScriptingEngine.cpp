#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <pybind11/embed.h>
#include <iostream>

#include "ScriptingEngine.h"

#include "Arcane/Core/InputManager.h"
#include "Arcane/ECS/Entity.h"

namespace py = pybind11;

namespace Arcane
{
	PYBIND11_EMBEDDED_MODULE(ArcanePythonModule, m) 
	{
		//////////////////////////////////////////////
		//// Maths Classes
		//////////////////////////////////////////////
		py::class_<glm::vec3>(m, "Vec3")
			.def(py::init<float, float, float>())
			.def_readwrite("x", &glm::vec3::x)
			.def_readwrite("y", &glm::vec3::y)
			.def_readwrite("z", &glm::vec3::z);
		
		//////////////////////////////////////////////
		//// Component Classes
		//////////////////////////////////////////////
		py::class_<TransformComponent>(m, "TransformComponent")
			.def(py::init<>())
			.def_readwrite("pos", &TransformComponent::pos)
			.def_readwrite("rotation", &TransformComponent::rotation)
			.def_readwrite("scale", &TransformComponent::scale);

		
		//////////////////////////////////////////////
		//// Entity Class
		//////////////////////////////////////////////


		//////////////////////////////////////////////
		//// Core Classes
		//////////////////////////////////////////////
		py::class_<InputManager>(m, "InputManager")
			.def("GetKeyPressed", &InputManager::GetKeyPressed)
			.def("GetKeyReleased", &InputManager::GetKeyReleased);
	}

	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		py::initialize_interpreter();
		
		py::print("Hello from python interp");

		// Append Working Dir to path
		py::module_ sys = py::module_::import("sys");
		sys.attr("path").attr("append")("C:\\Projects\\BasicGame\\Scripts");
		auto arcanePythonModule = py::module::import("ArcanePythonModule");
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
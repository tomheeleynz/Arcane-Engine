#include <iostream>

#include "ScriptingEngine.h"

#include "Arcane/Core/InputManager.h"
#include "Arcane/ECS/Entity.h"

namespace py = pybind11;

namespace Arcane
{
	PYBIND11_EMBEDDED_MODULE(ArcanePythonModule, m) 
	{
		m.doc() = "Hello Doc String";

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
		//// Core Classes 
		//////////////////////////////////////////////
		py::class_<InputManager>(m, "InputManager")
			.def("GetKeyPressed", &InputManager::GetKeyPressed)
			.def("GetKeyReleased", &InputManager::GetKeyReleased);

		py::class_<Entity>(m, "Entity")
			.def("GetTransformComponent", &Entity::GetComponent<TransformComponent>)
			.def("HasTransformComponent", &Entity::HasComponent<TransformComponent>);
	}

	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
		py::initialize_interpreter();

		PyObject* sysPath = PySys_GetObject((char*)"path");
		if (!sysPath)
			PyErr_Print();
		
		int appendList = PyList_Append(sysPath, PyUnicode_FromString("C:\\Projects\\BasicGame\\Scripts"));
		if (appendList != 0)
			PyErr_Print();
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

	void ScriptingEngine::AddScriptLocation(std::string scriptLocation)
	{
		bool is_in = m_ScriptLocations.find(scriptLocation) != m_ScriptLocations.end();

		if (!is_in) {
			m_ScriptLocations.insert(scriptLocation);
			m_SystemModule.attr("path").attr("append")(scriptLocation);
		}
	}
}
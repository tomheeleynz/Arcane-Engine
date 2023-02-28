#pragma once
#include <Python.h>
#include <string>
#include <filesystem>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include "Arcane/Assets/Asset.h"

namespace py = pybind11;

namespace Arcane
{
	class Script : public Asset
	{
	public:
		Script(std::string name);

		void OnStart();
		void OnUpdate(float deltaTime);
	private:
		py::module m_Module;
		
		// Class and Object of python class
		py::object m_Class;
		py::object m_Object;
	};
}
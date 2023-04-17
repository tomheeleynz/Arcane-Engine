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
	class Entity;
	class Scene;

	class Script : public Asset
	{
	public:
		Script(std::string name);

		void OnStart();
		void OnUpdate(float deltaTime);

		uint32_t GetEntityID();
		void SetEntityID(uint32_t id);

		void SetScriptScene(Scene* scene);
		Scene* GetScene();
	private:
		py::module m_Module;
		
		// Class and Object of python class
		py::object m_Class;
		py::object m_Object;

		uint32_t m_EntityID;
		Scene* m_ScriptScene;
	};
}
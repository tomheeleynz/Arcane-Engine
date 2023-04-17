#pragma once


#include <pybind11/embed.h>
#include <string>
#include <set>

namespace Arcane
{
	class ScriptingEngine
	{
	public:
		static ScriptingEngine* GetInstance();
		static void Shutdown();
		static void Init();
		static void CallFunc(PyObject* func);

		void AddScriptLocation(std::string scriptLocation);
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();
		void CallFuncImpl(PyObject* func);
	private:
		std::set<std::string> m_ScriptLocations;
		pybind11::module_ m_SystemModule;
	};
}
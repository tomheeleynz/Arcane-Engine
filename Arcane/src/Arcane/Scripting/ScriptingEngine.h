#pragma once

#include <Python.h>
#include <string>

namespace Arcane
{
	class ScriptingEngine
	{
	public:
		static ScriptingEngine* GetInstance();

		static void Init();
		static void CallFunc(PyObject* func);
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();
		void CallFuncImpl(PyObject* func);
	private:

	};
}
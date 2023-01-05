#pragma once

#ifdef _DEBUG
	#undef _DEBUG
		#include <python.h>
	#define _DEBUG
#else
	#include <python.h>
#endif

#include <string>

namespace Arcane
{
	class ScriptingEngine
	{
	public:
		static ScriptingEngine* GetInstance();

		static void Init();
		static PyObject* GetModule(std::string modulePath);
		static PyObject* GetFunc(const char* funcName, PyObject* module);
		static void CallFunc(PyObject* func);
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();
		PyObject* GetModuleImpl(std::string modulePath);
		PyObject* GetFuncImpl(const char* funcName, PyObject* module);
		void CallFuncImpl(PyObject* func);
	private:

	};
}
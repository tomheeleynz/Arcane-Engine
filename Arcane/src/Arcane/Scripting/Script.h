#pragma once
#include <Python.h>
#include <string>
#include <filesystem>

#include "Arcane/Assets/Asset.h"

namespace Arcane
{
	class Script : public Asset
	{
	public:
		Script(std::filesystem::path path);

		PyObject* GetStartFunction() { return m_StartFunction; }
		PyObject* GetUpdateFunction() { return m_UpdateFunction; }
	private:
		PyObject* m_Module;
		PyObject* m_StartFunction;
		PyObject* m_UpdateFunction;
	};
}
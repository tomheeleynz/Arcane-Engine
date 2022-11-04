#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

#include "ScriptingEngine.h"

namespace Arcane
{
	ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

	ScriptingEngine::ScriptingEngine()
	{
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
}
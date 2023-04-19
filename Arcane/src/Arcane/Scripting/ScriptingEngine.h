#pragma once

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
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();
	private:
	};
}
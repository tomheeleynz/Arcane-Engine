#pragma once

namespace Arcane
{
	class ScriptingEngine
	{
	public:
		static ScriptingEngine* GetInstance();

		static void Init();
	private:
		ScriptingEngine();
		static ScriptingEngine* s_Instance;

		void InitImpl();
	private:

	};
}
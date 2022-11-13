#pragma once

#include <thread>
#include <queue>
#include <functional>

namespace Arcane
{
	class AssetQueue
	{
	public:
		

		static AssetQueue* GetInstance();
	private:
		AssetQueue();
		static AssetQueue* s_Instance;

		void RunJob();
	private:
		std::thread m_AssetThread;
		std::queue<std::function<void()>> m_Jobs;
	};
}
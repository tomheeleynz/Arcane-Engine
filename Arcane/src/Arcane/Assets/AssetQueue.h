#pragma once

#include <thread>
#include <functional>
#include <queue>
#include <mutex>

namespace Arcane	
{
	class AssetQueue
	{
	public:
		static AssetQueue* GetInstance();

		static void QueueJob(const std::function<void()>& job);
		static void Shutdown();
	private:
		static AssetQueue* s_Instance;
		AssetQueue();

		void RunJob();
		void QueueJobImpl(const std::function<void()>& job);
		void ShutdownImpl();
	private:
		bool m_ShouldTerminate = false;
		std::thread m_AssetThread;
		std::mutex m_QueueMutex;
		std::condition_variable m_MutexCondition;
		std::queue<std::function<void()>> m_Jobs;
	};
}
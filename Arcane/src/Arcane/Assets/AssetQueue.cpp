#include "AssetQueue.h"

namespace Arcane
{
	AssetQueue* AssetQueue::s_Instance = nullptr;

	AssetQueue::AssetQueue()
	{

	}

	AssetQueue* AssetQueue::GetInstance()
	{
		if (s_Instance != nullptr)
			s_Instance = new AssetQueue();

		return s_Instance;
	}

	void AssetQueue::RunJob()
	{
		while (true) 
		{
			std::function<void()> job;
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				
				m_MutexCondition.wait(lock, [this] {
					return !m_Jobs.empty() || m_ShouldTerminate;
				});

				if (m_ShouldTerminate) {
					return;
				}

				job = m_Jobs.front();
				m_Jobs.pop();
			}
			job();
		}
	}

	void AssetQueue::QueueJob(const std::function<void()>& job)
	{
		GetInstance()->QueueJobImpl(job);
	}

	void AssetQueue::QueueJobImpl(const std::function<void()>& job)
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_Jobs.push(job);
		m_MutexCondition.notify_one();
	}

	void AssetQueue::Shutdown()
	{
		GetInstance()->ShutdownImpl();
	}

	void AssetQueue::ShutdownImpl()
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_ShouldTerminate = true;
		m_MutexCondition.notify_all();
		m_AssetThread.join();
	}
}
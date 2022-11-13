#include "AssetQueue.h"

namespace Arcane
{
	AssetQueue* AssetQueue::s_Instance = nullptr;

	AssetQueue::AssetQueue()
	{
		
	}

	AssetQueue* AssetQueue::GetInstance()
	{
		if (!s_Instance)
			s_Instance = new AssetQueue();

		return s_Instance;
	}

	void AssetQueue::RunJob()
	{
		while (true) {
			std::function<void()> job;
			job = m_Jobs.front();
			m_Jobs.pop();
			job();
		}
	}
}
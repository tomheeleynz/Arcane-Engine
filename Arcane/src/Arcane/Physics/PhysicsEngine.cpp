#include "PhysicsEngine.h"

namespace Arcane
{
	PhysicsEngine* PhysicsEngine::s_Instance = nullptr;

	PhysicsEngine::PhysicsEngine()
	{
		static physx::PxDefaultErrorCallback gDefaultErrorCallback;
		static physx::PxDefaultAllocator gDefaultAllocatorCallback;

		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
			gDefaultErrorCallback);
		
		if (!m_Foundation)
			std::cout << "PxCreateFoundation failed!" << std::endl;
	}

	PhysicsEngine* PhysicsEngine::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new PhysicsEngine();

		return s_Instance;
	}

	void PhysicsEngine::Init()
	{
		GetInstance();
	}
}
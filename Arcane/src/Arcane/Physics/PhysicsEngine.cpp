#include "PhysicsEngine.h"

namespace Arcane
{
	#define PVD_HOST "127.0.0.1"

	PhysicsEngine* PhysicsEngine::s_Instance = nullptr;

	PhysicsEngine::PhysicsEngine()
	{
		static physx::PxDefaultErrorCallback gDefaultErrorCallback;
		static physx::PxDefaultAllocator gDefaultAllocatorCallback;

		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
			gDefaultErrorCallback);
		
		if (!m_Foundation)
			std::cout << "PxCreateFoundation failed!" << std::endl;

		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, nullptr);

		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	}

	physx::PxPhysics* PhysicsEngine::GetPhysics()
	{
		return GetInstance()->GetPhysicsImpl();
	}

	physx::PxDefaultCpuDispatcher* PhysicsEngine::GetDispatcher()
	{
		return GetInstance()->GetDispatcherImpl();
	}
	
	physx::PxDefaultCpuDispatcher* PhysicsEngine::GetDispatcherImpl()
	{
		return m_CpuDispatcher;
	}

	physx::PxPhysics* PhysicsEngine::GetPhysicsImpl()
	{
		return m_Physics;
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
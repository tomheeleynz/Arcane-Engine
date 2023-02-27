#pragma once

#include <iostream>
#include "PxPhysicsAPI.h"

namespace Arcane
{
	class PhysicsEngine
	{
	public:
		static PhysicsEngine* GetInstance();

		static void Init();
		static physx::PxPhysics* GetPhysics();
		static physx::PxDefaultCpuDispatcher* GetDispatcher();
	private:
		PhysicsEngine();
		static PhysicsEngine* s_Instance;

		physx::PxPhysics* GetPhysicsImpl();
		physx::PxDefaultCpuDispatcher* GetDispatcherImpl();
	private:
		physx::PxFoundation* m_Foundation;
		physx::PxPhysics* m_Physics;
		physx::PxDefaultCpuDispatcher* m_CpuDispatcher;
		bool m_RecordMemoryAllocation = true;
	};
}
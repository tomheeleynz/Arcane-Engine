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
	private:
		PhysicsEngine();
		static PhysicsEngine* s_Instance;

	private:
		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_Pvd;
		bool m_RecordMemoryAllocation = true;
	};
}
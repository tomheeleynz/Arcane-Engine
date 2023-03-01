#pragma once

#include <glm/glm.hpp>
#include "Arcane/Physics/PhysicsEngine.h"

namespace Arcane
{
	class RigidBody
	{
	public:
		RigidBody(glm::vec3 position);
	
		physx::PxRigidActor* GetRigidBody() { return m_RigidBody; };

		// Return Position of rigid body 
		glm::vec3 GetPosition();

		// Get and Set Mass
		float GetMass() { return m_Mass; }
		void SetMass(float mass);
	private:
		physx::PxRigidDynamic* m_RigidBody;
		physx::PxTransform m_Transform;
		float m_Mass = 0;
	};
}
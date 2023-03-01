#pragma once

#include <glm/glm.hpp>
#include "Arcane/Physics/PhysicsEngine.h"

namespace Arcane
{
	class RigidBody
	{
	public:
		RigidBody(glm::vec3 position);
		
		// Get Rigidbody structure
		physx::PxRigidActor* GetRigidBody() { return m_RigidBody; };

		// Return Position of rigid body 
		glm::vec3 GetPosition();

		// Get and Set Mass
		float GetMass() { return m_Mass; }
		void SetMass(float mass);

		// Linear Velocity
		glm::vec3 GetLinearVelocity();
		void SetLinearVelocity(glm::vec3 linearVelocity);


	private:
		physx::PxRigidDynamic* m_RigidBody;
		physx::PxTransform m_Transform;
		float m_Mass = 0;
	};
}
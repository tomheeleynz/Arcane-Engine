#include "RigidBody.h"

namespace Arcane
{
	RigidBody::RigidBody(glm::vec3 pos = {0.0f, 0.0f, 0.0f})
	{
		m_Transform = physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z));
		m_RigidBody = Arcane::PhysicsEngine::GetPhysics()->createRigidDynamic(m_Transform);
		
		m_Mass = 1.0f;
		physx::PxRigidBodyExt::updateMassAndInertia(*m_RigidBody, 1.0f);
	}

	glm::vec3 RigidBody::GetPosition()
	{
		return { 
			m_RigidBody->getGlobalPose().p.x, 
			m_RigidBody->getGlobalPose().p.y,
			m_RigidBody->getGlobalPose().p.z 
		};
	}

	void RigidBody::SetMass(float mass) {
		m_Mass = mass;
		physx::PxRigidBodyExt::updateMassAndInertia(*m_RigidBody, mass);
	}

	glm::vec3 RigidBody::GetLinearVelocity()
	{
		return { 
			m_RigidBody->getLinearVelocity().x,
			m_RigidBody->getLinearVelocity().y,
			m_RigidBody->getLinearVelocity().z
		};
	}
	
	void RigidBody::SetLinearVelocity(glm::vec3 velocity) 
	{
		m_RigidBody->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}
}
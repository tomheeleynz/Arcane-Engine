#pragma once

#include <glm/glm.hpp>

#include "Arcane/Physics/PhysicsEngine.h"

namespace Arcane
{
	class BoxCollider
	{
	public:
		BoxCollider(glm::vec3 min, glm::vec3 max, physx::PxRigidActor* actor);

		physx::PxShape* GetBox() { return m_Box; }
	private:
		physx::PxShape* m_Box;
	};
}
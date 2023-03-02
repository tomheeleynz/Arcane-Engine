#include "BoxCollider.h"

namespace Arcane
{
	BoxCollider::BoxCollider(glm::vec3 min, glm::vec3 max, physx::PxRigidActor* actor)
	{
		// Calculate Half Extents
		float xHalfExtent = (max.x - min.x) / 2.0f;
		float yHalfExtent = (max.y - min.y) / 2.0f;
		float zHalfExtent = (max.z - min.z) / 2.0f;

		// Create default physics material (will eventually be an extra component?)
		physx::PxMaterial* material = PhysicsEngine::GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);

		// Create Shape
		m_Box = PhysicsEngine::GetPhysics()->createShape(physx::PxBoxGeometry(xHalfExtent, yHalfExtent, zHalfExtent), *material, true);
		actor->attachShape(*m_Box);
	}
}
#include "Kinetics/Structures/PhysicsBody.h"

namespace Kinetics
{
	PhysicsBody::PhysicsBody(Shape* shape)
	{

	}

	void PhysicsBody::SetShape(Shape* shape)
	{
		m_Shape = shape;
	}

	Shape* PhysicsBody::GetShape()
	{
		return m_Shape;
	}

	void PhysicsBody::SetMass(float mass)
	{
		m_MassData.mass = mass;
		m_MassData.inv_mass = 1.0f / mass;
	}
}
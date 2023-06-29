#pragma once

#include "Kinetics/Structures/Shape.h"
#include "Kinetics/Structures/DataStructures.h"

namespace Kinetics
{
	class PhysicsBody
	{
	public:
		PhysicsBody(Shape* shape);

		void SetShape(Shape* shape);
		Shape* GetShape();

		void SetMass(float mass);

		float GetMass() { return m_MassData.mass; };
		float GetInvMass() { return m_MassData.inv_mass; }
	private:
		Shape* m_Shape;
		MassData m_MassData;
	};
}
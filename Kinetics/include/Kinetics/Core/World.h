#pragma once

#include <vector>

#include "Kinetics/Structures/DataStructures.h"
#include "Kinetics/Structures/PhysicsBody.h"
#include "PhysicsClock.h"

namespace Kinetics
{
	class World 
	{
	public:
		World();
		~World();

		PhysicsBody* CreatePhysicsBody(ShapeDef shapeDef, PhysicsBodyDef physicsBodyDef);
		
		void TimeStep(float timestep);
	private:
		std::vector<PhysicsBody> m_PhysicsBodies;
		PhysicsClock* m_Clock;
		float m_Accumulator = 0.0f;
	};
}
#include "Kinetics/Core/World.h"

namespace Kinetics
{
	World::World()
	{
		m_Clock = new PhysicsClock();
		m_Accumulator = 0.0f;
	}

	World::~World()
	{
		delete m_Clock;
	}

	PhysicsBody* World::CreatePhysicsBody(ShapeDef shapeDef, PhysicsBodyDef physicsBodyDef)
	{
		// Create Shape 
		Shape* newShape = new Shape(shapeDef.type);

		// Add Mass to shape
		PhysicsBody* newPhysicsBody = new PhysicsBody(newShape);
		newPhysicsBody->SetMass(physicsBodyDef.mass);
		return newPhysicsBody;
	}

	void World::TimeStep(float timestep) 
	{
		// Will need a clock
		m_Clock->Process();
		float deltaTime = m_Clock->GetDeltaTick() / 1000.0f;

		m_Accumulator += deltaTime;
		if (m_Accumulator > 0.2f)
			m_Accumulator = 0.2f;

		while (m_Accumulator > timestep) {
			m_Accumulator -= timestep;
		}
	}
}
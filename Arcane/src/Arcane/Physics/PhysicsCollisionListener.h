#pragma once

#include <Kinetics/Core/ContactListener.h>

namespace Arcane
{
	class PhysicsCollisionListener : public Kinetics::ContactListener
	{
	public:

		virtual void OnContact(Kinetics::Pair& pair) override;
	private:
	
	};
}
#include "PhysicsCollisionListener.h"
#include "Arcane/ECS/Entity.h"

namespace Arcane
{
	void PhysicsCollisionListener::OnContact(Kinetics::Pair& pair)
	{
		Entity* aEntity = static_cast<Entity*>(pair.a->GetUserData());
		Entity* bEntity = static_cast<Entity*>(pair.b->GetUserData());

		if (aEntity->HasComponent<ScriptComponent>())
		{
			Script* aScript = aEntity->GetComponent<ScriptComponent>().script;
			aScript->OnContact();
		}

		if (bEntity->HasComponent<ScriptComponent>())
		{
			Script* bScript = bEntity->GetComponent<ScriptComponent>().script;
			bScript->OnContact();
		}
	}
}
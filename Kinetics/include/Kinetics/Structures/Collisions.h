#pragma once

#include <math.h>
#include "Kinetics/Structures/DataStructures.h"

namespace Kinetics 
{
	bool AABBvsAABB(AABB& a, AABB& b) {
		if (a.max.x < b.min.x || a.min.x > b.max.x)
			return false;
	
		if (a.max.y < b.min.y || a.min.y > b.max.y)
			return false;

		return true;
	};

	bool CirclevsCircle(Circle a, Circle b)
	{
		float r = a.radius + b.radius;
		r *= r;
		return r < ((a.position.x + b.position.x) * (a.position.x + b.position.x)) 
				+ ((a.position.y + b.position.y) * (a.position.y + b.position.y));
	}
}
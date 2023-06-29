#pragma once

#include "Kinetics/Maths/Vector.h"

namespace Kinetics 
{
	// Shape Types
	enum class ShapeType 
	{
		BOX,
		CIRCLE
	};

	// No Rotation Yet
	struct MassData
	{
		float mass;
		float inv_mass;
	};

	// AABB
	struct AABB
	{
		Vec2 min;
		Vec2 max;
	};

	// Circle
	struct Circle
	{
		float radius;
		Vec2 position;
	};

	////////////////////////////////////////////////////////////
	/// Shape and Physics Body Definitions
	////////////////////////////////////////////////////////////
	struct ShapeDef 
	{
		ShapeType type;
	};

	struct PhysicsBodyDef 
	{
		float mass;
	};
}


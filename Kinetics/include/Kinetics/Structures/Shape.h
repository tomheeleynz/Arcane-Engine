#pragma once

#include "DataStructures.h"

namespace Kinetics 
{
	class Shape
	{
	public:
		Shape(ShapeType type);

		// Get Shape Type 
		ShapeType GetShapeType() { return m_Type; }
	private:
		ShapeType m_Type;
	};
}
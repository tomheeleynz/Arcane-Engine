#pragma once

#include <initializer_list>

namespace Arcane {

	enum class VertexType
	{
		float1,
		float2,
		float3,
		float4,
		integer
	};


	class VertexDescriptor
	{
	public:

		static VertexDescriptor* Create(std::initializer_list<VertexType> vertexTypes);
	private:

	};
}
#pragma once

#include <initializer_list>


enum class VertexType
{
	float1, 
	float2, 
	float3,
	float4
};


class VertexDescriptor
{
public:

	static VertexDescriptor* Create(std::initializer_list<VertexType> vertexTypes);
private:

};
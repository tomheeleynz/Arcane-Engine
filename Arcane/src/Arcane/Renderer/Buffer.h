#pragma once

#include <iostream>

////////////////////////////////////////////////////////
//// Index Buffer
////////////////////////////////////////////////////////
class IndexBuffer
{
public:
	virtual uint32_t GetCount() = 0;

	static IndexBuffer* Create(void* data, uint32_t count);
private:


};


////////////////////////////////////////////////////////
//// Vertex Buffer
////////////////////////////////////////////////////////
class VertexBuffer
{
public:
	virtual void AddIndexBuffer(IndexBuffer* _indexBuffer) = 0;
	virtual IndexBuffer* GetIndexBuffer() = 0;

	static VertexBuffer* Create(void* data, uint32_t size);
private:

};


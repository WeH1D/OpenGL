#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
private:
	unsigned int _rendererID;
public:
	VertexArray();
	~VertexArray();


	void Bind() const;
	void Unbind() const;


	void AddLayout( VertexBuffer& vb, IndexBuffer& ib, VertexBufferLayout& layout);
};

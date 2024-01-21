#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferLayoutElement
{
	unsigned int type;
	unsigned int count;
	int normalized;
	 
	static unsigned int GetSizeOfType(int type)
	{
		switch (type)
		{
			case GL_BYTE: return 4;
			case GL_FLOAT: return 4;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferLayoutElement> _elements;
	unsigned int _stride;
public:
	VertexBufferLayout() {};
	~VertexBufferLayout() {};

	inline std::vector<VertexBufferLayoutElement> GetElements() const { return _elements; };
	inline int GetStride() const { return _stride; };

	template<typename T>
	void Push(unsigned int count)
	{
		_elements.push_back({ GL_BYTE, count, GL_TRUE,});
		_stride += VertexBufferLayoutElement::GetSizeOfType(GL_BYTE) * count;
	}

	template<>
	void Push<float>(unsigned int count)
	{
		_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		_stride += VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT) * count;
	}
};

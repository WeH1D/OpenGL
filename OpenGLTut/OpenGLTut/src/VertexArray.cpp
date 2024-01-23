#include "VertexArray.h"
#include "Utils.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &_rendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &_rendererID));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(_rendererID));
		
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddLayout(VertexBuffer& vb, IndexBuffer& ib, VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	ib.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size();i++)
	{
		auto element = elements[i];
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void *)offset));
		offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
	}
}

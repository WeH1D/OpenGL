#pragma once
#include "Shader.h"
#include "VertexArray.h"

class Renderer
{
private:
public:
	Renderer();
	~Renderer();

	void Draw(VertexArray& va, unsigned int elementCount, Shader& shader) const;
	void Clear() const;
};

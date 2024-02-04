#pragma once
#include "Shader.h"
#include "VertexArray.h"

enum DrawMode {
	ELEMENTS, ARRAYS
};

class Renderer
{
private:
public:
	Renderer();
	~Renderer();

	void Draw(DrawMode mode, VertexArray& va, unsigned int count, Shader& shader) const;
	void Clear() const;
};

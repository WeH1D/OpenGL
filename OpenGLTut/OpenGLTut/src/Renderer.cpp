#include "Renderer.h"
#include "Utils.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(DrawMode mode, VertexArray& va, unsigned int count, Shader& shader) const
{
	va.Bind();
	shader.Bind();
	
	if (mode == DrawMode::ELEMENTS) 
	{
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	}
	else if(mode == DrawMode::ARRAYS)
	{
		GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
	}
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

#include "Renderer.h"
#include "Utils.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(VertexArray& va, unsigned int elementCount, Shader& shader) const
{
	va.Bind();
	shader.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

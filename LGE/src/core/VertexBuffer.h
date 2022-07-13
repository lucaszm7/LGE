#pragma once
#include <GL/glew.h>

// Create a vertex buffer in the GPU MEM, pass the data in and Binded
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, GLenum mode = GL_STATIC_DRAW);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>

// Create a vertex buffer in the GPU MEM, pass the data in and Binded
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, GLenum mode = GL_DYNAMIC_DRAW);
	~VertexBuffer();
	void New();
	void Delete();
	void Resize(size_t new_size, void* data = nullptr, GLenum mode = GL_DYNAMIC_DRAW);
	size_t Size() const;
	void Bind() const;
	void Unbind() const;
};
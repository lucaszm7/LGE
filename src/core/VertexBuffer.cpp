#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // Gen a buffer in the graphics card, and return the ID of that buffer
    glGenBuffers(1, &m_RendererID);
    // Tell the OpenGL what type is that buffer and selected
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    // As we know the data we want, we can provide with data right way
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


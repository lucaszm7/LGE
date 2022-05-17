#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum mode)
    : m_Count(count)
{
    // Gen a buffer in the graphics card, and return the ID of that buffer
    glGenBuffers(1, &m_RendererID);
    // Tell the OpenGL what type is that buffer and selected
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    // As we know the data we want, we can provide with data right way
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, mode);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


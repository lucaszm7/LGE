#include "IndexBuffer.h"
#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum mode)
    : m_Count(count)
{
    // Gen a buffer in the graphics card, and return the ID of that buffer
    glGenBuffers(1, &m_RendererID);
    std::cout << "Creating Index Buffer - " << m_RendererID << "\n";
    // Tell the OpenGL what type is that buffer and selected
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    // As we know the data we want, we can provide with data right way
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, mode);
}

IndexBuffer::~IndexBuffer()
{
    std::cout << "Deleting Index Buffer - " << m_RendererID << "\n";
    Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::New()
{
    // Gen a buffer in the graphics card, and return the ID of that buffer
    glGenBuffers(1, &m_RendererID);
    // std::cout << "Creating Vertex Buffer - " << m_RendererID << "\n";
    // Tell the OpenGL what type is that buffer and selected
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Delete()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    GLint size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    std::cout << "Deleting Index Buffer - " << m_RendererID << " of size: " << size << "\n";
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Resize(size_t new_size, void* data, GLenum mode)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, new_size * sizeof(unsigned int), data, mode);
}

size_t IndexBuffer::Size() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    GLint size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    return size;
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


#pragma once

#include<GL/glew.h>

#define ASSERT() __debugbreak();

#define SCREEN_WIDTH 960.0f
#define SCREEN_HEIGHT 540.0f

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam);


class Renderer
{
public:
    void Clear() const;
    const void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const;
private:

};
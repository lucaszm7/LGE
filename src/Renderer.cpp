#include <iostream>

#include "Renderer.h"

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "\n\033[1;31m[GL DEBUG CALLBACK ERROR]\033[0m\n" << "\033[1;33m" << message << "\033[0m" << std::endl;
    ASSERT(true);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

const void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    vao.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

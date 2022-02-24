#pragma once

#include<GL/glew.h>

#define ASSERT() __debugbreak();

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam);
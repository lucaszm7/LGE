#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT() __debugbreak();

#define SCREEN_WIDTH 960.0f
#define SCREEN_HEIGHT 540.0f

class VertexArray;
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Renderer
{
public:
    static void Clear();
    static void ClearColor(float v0, float v1, float v2, float v3);
    static void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader);
    static GLFWwindow* SetupGLFW();
    static void SetupGLEW();
    static void SetupImGui(GLFWwindow* window);
    static void CreateImGuiFrame();
    static void UpdateImGui();
    static void UpdateGLFW(GLFWwindow* window);
    static void CleanUpImGui();
    static void CleanUpGLFW(GLFWwindow* window);

private:

};
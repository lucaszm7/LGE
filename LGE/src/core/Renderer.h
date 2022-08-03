#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/gtx/norm.hpp>

#define ASSERT() __debugbreak();

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

namespace LGE
{
    int GetKey(int key); // GLFW_KEY_E
    void GetCursorPos(double& xpos, double& ypos);
    int GetMouseButton(int mouseButton = GLFW_MOUSE_BUTTON_LEFT);
}



class Renderer
{
public:
public:
    static void Init();
    static int WindowShouldClose();
    static void SetWindowTitle(const char* name);
    static void Clear();
    static void ClearColor(float v0, float v1, float v2, float v3);
    static void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader);
    static GLFWwindow* SetupGLFW();
    static void SetupGLEW();
    static void SetupImGui();
    static void CreateImGuiFrame();
    static void UpdateImGui();
    static void UpdateGLFW();
    static void CleanUpImGui();
    static void CleanUpGLFW();
    static constexpr unsigned int GetScreenWidth() { return SCREEN_WIDTH; }
    static constexpr unsigned int GetScreenHeight() { return SCREEN_HEIGHT; }
};

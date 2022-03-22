#include <iostream>

#include "Renderer.h"

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const char* message, const void* userParam)
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

/*
    Setup ImGui Stuff
    Including: Docking, Font size, DarkMode, OpenGL, GLFW
*/
void Renderer::SetupImGui(GLFWwindow* window) const
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontDefault();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

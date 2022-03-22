#include <iostream>

#include "Renderer.h"

void GLAPIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const char* message, const void* userParam)
{
    std::cout << "\n\033[1;31m[GL DEBUG CALLBACK ERROR]\033[0m\n" << "\033[1;33m" << message << "\033[0m" << std::endl;
    ASSERT(true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    {
        // Pick the main monitor
        GLFWmonitor* fullScreenMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* fullScreenMode = glfwGetVideoMode(fullScreenMonitor);
        GLFWmonitor* monitor = glfwGetWindowMonitor(window);
        if (!monitor)
            glfwSetWindowMonitor(window, fullScreenMonitor, 0, 0, fullScreenMode->width, fullScreenMode->height, fullScreenMode->refreshRate);
        else
            glfwSetWindowMonitor(window, NULL, 200, 200, SCREEN_WIDTH, SCREEN_HEIGHT, fullScreenMode->refreshRate);
    }

    if (key == GLFW_KEY_EQUAL && action == GLFW_RELEASE)
    {
        std::cout << "Equals Pressed!\n";
    }

}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader)
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
void Renderer::SetupImGui(GLFWwindow* window)
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

void Renderer::SetupGLEW()
{
    /* Setup OpenGL */
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallBack, 0);
    std::cout << "\033[1;32m" << glGetString(GL_VERSION) << "\033[0m" << std::endl;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

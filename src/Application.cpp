#include <iostream>
#include <string>
#include <array>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"
#include "GLCore.h"

#include "App.h"
#include "TestDemo.h"
#include "TestClearColor.h"


class SudoTest : public App::AppBase
{
private:

    std::unique_ptr<Shader> m_Shader;

    float posA[2] = { -0.5f, -0.5f };
    float posB[2] = { -0.5f, -0.5f };
    float posP[2] = { -0.5f, -0.5f };
    float m_R = 50.0f;

    Quad a;
    Tri b;
    Point p;
    Line l;

public:
    SudoTest()
        :a(posA[0], posA[1]),
        b(posB[0], posB[1]),
        p(0.0f, 0.0f),
        l()
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
    }
    ~SudoTest()
    {
        m_Shader->Unbind();
    }

    void OnUpdate(float deltaTime) override
    {

    }
    void OnRender() override
    {
        m_Shader->Bind();

        a.Set(posA[0], posA[1]);
        a.Draw();

        b.Set(posB[0], posB[1]);
        b.Draw();

        p.Set(posP[0], posP[1], glm::vec4( 1.0f, 0.0f ,0.0f,1.0f ), m_R);
        p.Draw();

        l.Draw();
    }
    void OnImGuiRender() override
    {
        ImGui::DragFloat2(" Pos A ", posA, 0.1f, -1.0f, 1.0f);
        ImGui::DragFloat2(" Pos B ", posB, 0.1f, -1.0f, 1.0f);
        ImGui::DragFloat2(" Pos P ", posP, 0.1f, -1.0f, 1.0f);
        ImGui::DragFloat("Radius", &m_R, 10.0f, 10.0f, 300.0f);
    }
};

int main(void)
{
    GLFWwindow* window = Renderer::SetupGLFW();
    Renderer::SetupGLEW();
    Renderer::SetupImGui(window);

    App::AppBase* currentApp = nullptr;
    App::AppMenu* mainMenu = new App::AppMenu(currentApp);
    currentApp = mainMenu;

    mainMenu->RegisterApp<App::TestClearColor>("Clear Color");
    mainMenu->RegisterApp<App::TestDemo>("Base Demo");
    mainMenu->RegisterApp<SudoTest>("Polygon Test");

    glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        Renderer::Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Render here */
        if (currentApp)
        {
            ImGui::Begin("App Menu");
            if (currentApp != mainMenu && ImGui::Button("<- Main Menu"))
            {
                delete currentApp;
                currentApp = mainMenu;
                glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
            }
            currentApp->OnUpdate(0.0f);
            currentApp->OnRender();
            currentApp->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(window);

        glfwSwapBuffers(window);
        glfwSetKeyCallback(window, key_callback);
        glfwPollEvents();
    }

    if (currentApp != mainMenu)
        delete mainMenu;
    delete currentApp;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
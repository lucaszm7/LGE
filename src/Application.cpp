#include <iostream>
#include <string>
#include <array>
#include <vector>

#include <cstdlib>
#include <ctime>

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
    std::unique_ptr<SPoint> m_SPoints;
    std::unique_ptr<SLine> m_SLines;
    std::unique_ptr<SPolygon> m_SPolygon;
    std::vector<Point2D> m_Points2D;

public:
    SudoTest()
        :a(posA[0], posA[1]),
        b(posB[0], posB[1])
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

        srand(time(0));
        for (int i = 0; i < 50; ++i)
        {
            Point2D p;
            p.Position = 
            { 
                -1.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f))), 
                -1.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f))) 
            };
            m_Points2D.push_back(p);
        }

        Point2D pa, pb, pc;
        pa.Position = { 0.0f, 0.0f };
        pb.Position = { 0.5f, 0.0f };
        pc.Position = { -0.5f, 0.0f };
        m_Points2D.push_back(pa);
        m_Points2D.push_back(pb);
        m_Points2D.push_back(pc);

        m_SPoints = std::make_unique<SPoint>(&m_Points2D[0], m_Points2D.size());
        m_SLines = std::make_unique<SLine>(&m_Points2D[0], m_Points2D.size());
        m_SPolygon = std::make_unique<SPolygon>(&m_Points2D[0], m_Points2D.size());
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

        m_Shader->SetUniform1i("u_color", 1);
        m_Shader->SetUniform4f("u_Color", 0.0f, 1.0f, 1.0f, 1.0f);
        m_SPolygon->Draw();
        m_Shader->SetUniform1i("u_color", 0);

        a.Set(posA[0], posA[1]);
        a.Draw();

        b.Set(posB[0], posB[1]);
        b.Draw();

        m_SPoints->Draw();
        m_Shader->SetUniform1i("u_color", 1);
        m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);
        m_SLines->Draw();
        m_Shader->SetUniform1i("u_color", 0);

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
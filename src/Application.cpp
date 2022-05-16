// Draw Stuff
#include "GLCore.h"
#include "Geometry.h"
#include "Scene.h"

// Example Scenes
#include "TestDemo.h"
#include "TestClearColor.h"
#include "PolygonTest.h"
#include "ConvexHull.h"


class QuadTree : public Scene::Scene_t
{
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<SPoint> m_SPoints;

public:
    QuadTree(int argc = 0, char** argv = nullptr) 
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_SPoints = std::make_unique<SPoint>();
    }

    ~QuadTree()
    {
        m_Shader->Unbind();
    }

    void OnUpdate(float deltaTime) override
    {
        
    }

    void OnRender() override
    {
        // m_Shader->Bind();
        m_SPoints->Draw();
    }

    void OnImGuiRender() override 
    {

    }

};

int main(int argc, char** argv)
{
    GLFWwindow* window = Renderer::SetupGLFW();
    Renderer::SetupGLEW();
    Renderer::SetupImGui(window);

    Scene::Scene_t* currentApp = nullptr;
    Scene::Menu* mainMenu = new Scene::Menu(currentApp);
    currentApp = mainMenu;

    mainMenu->RegisterApp<Scene::TestClearColor>("Clear Color Test");
    mainMenu->RegisterApp<Scene::TestDemo>("Texture Test");
    mainMenu->RegisterApp<PolygonTest>("Polygon Test");
    mainMenu->RegisterApp<ConvexHull>("ConvexHull");
    mainMenu->RegisterApp<QuadTree>("QuadTree");

    Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        Renderer::Clear();
        Renderer::CreateImGuiFrame();

        /* Render here */
        ImGui::Begin(mainMenu->c_SceneName.c_str());
        if (currentApp != mainMenu && ImGui::Button("<- Main Menu"))
        {
            delete currentApp;
            currentApp = mainMenu;
            mainMenu->c_SceneName = "Main Menu";
            Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);
        }
        currentApp->OnUpdate(0.0f);
        currentApp->OnRender();
        currentApp->OnImGuiRender();
        ImGui::End();
        /* Render Ends */

        Renderer::UpdateImGui();
        Renderer::UpdateGLFW(window);
    }

    if (currentApp != mainMenu)
        delete mainMenu;
    delete currentApp;

    Renderer::CleanUpImGui();
    Renderer::CleanUpGLFW(window);
    
    return 0;
}
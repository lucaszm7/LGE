#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLCore.h"
#include "Geometry.h"
#include "Scene.h"

#include "TestDemo.h"
#include "TestClearColor.h"
#include "PolygonTest.h"

class ConvexHull : public Scene::Scene_t
{
private:
    std::unique_ptr<Shader> m_Shader;

    std::unique_ptr<SPoint> m_SPoints;
    std::unique_ptr<SLine> m_SLines;
    std::unique_ptr<SPolygon> m_SPolygon;
    std::vector<Point2D> m_PointsIn;
    std::vector<Point2D> m_PointsOut;
    std::vector<Point2D> m_rand;
public:

    bool right_turn(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
        float val = (b.x - a.x) * (c.y - a.y) - 
                    (b.y - a.y) * (c.x - a.x);
        if (val <= 0)
            return true;       // right turn
        return false;          // left turn
    }

    ConvexHull() 
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

        // srand(time(0));
        for (int i = 0; i < 50; ++i)
        {
            Point2D p;
            p.Position =
            {
                -1.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f))),
                -1.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)))
            };

            m_PointsIn.push_back(p);
        }


        m_PointsIn.push_back(Point2D(-0.7f,  0.0f));
        m_PointsIn.push_back(Point2D(-0.5f, -0.2f));
        m_PointsIn.push_back(Point2D(-0.4f, +0.2f));
        m_PointsIn.push_back(Point2D(-0.1f, +0.2f));
        m_PointsIn.push_back(Point2D( 0.0f, -0.2f));
        m_PointsIn.push_back(Point2D(+0.2f, +0.6f));
        m_PointsIn.push_back(Point2D(+0.4f, +0.2f));
        m_PointsIn.push_back(Point2D(+0.5f, -0.1f));
        m_PointsIn.push_back(Point2D(+0.8f,  0.0f));
        std::sort(m_PointsIn.begin(), m_PointsIn.end());


        /*size_t n = m_PointsIn.size(), k = 0;
        std::vector<Point2D> H(2 * n);

        for (size_t i = 0; i < n; ++i)
        {
            while (k >= 2 && right_turn(H[k - 2].Position, H[k - 1].Position, m_PointsIn[i].Position))
                k--;
            H[k] = m_PointsIn[i];
            k++;
        }

        for (size_t i = n - 1, t = k + 1; i > 0; --i) {
            while (k >= t && right_turn(H[k - 2].Position, H[k - 1].Position, m_PointsIn[i - 1].Position)) 
                k--;
            H[k] = m_PointsIn[i - 1];
            k++;
        }

        H.resize(k - 1);
        m_PointsOut = H;*/


        std::vector<Point2D> H(2 * m_PointsIn.size());
        unsigned int i = 0, k = 0;
        while (i < m_PointsIn.size())
        {
            while (k >= 2 && !right_turn(H[k - 2].Position, H[k - 1].Position, m_PointsIn[i].Position))
                --k;
            H[k] = m_PointsIn[i];
            ++k;
            ++i;
        }
        
        i = m_PointsIn.size() - 1;
        unsigned int t = k + 1;
        while (i > 0)
        {
            while (k >= t && !right_turn(H[k - 2].Position, H[k - 1].Position, m_PointsIn[i - 1].Position))
                --k;
            H[k] = m_PointsIn[i - 1];
            k++;
            i--;
        }

        H.resize(k-1);
        m_PointsOut = H;

        m_SPoints = std::make_unique<SPoint>(&m_PointsIn[0], m_PointsIn.size());
        m_SLines = std::make_unique<SLine>(&m_PointsOut[0], m_PointsOut.size());
        m_SPolygon = std::make_unique<SPolygon>(&m_PointsOut[0], m_PointsOut.size());
    }

    ~ConvexHull()
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
        
        m_Shader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        m_SPoints->Draw(&m_PointsIn[0], m_PointsIn.size());

        m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);
        m_SLines->Draw(GL_LINE_LOOP);

        m_Shader->SetUniform4f("u_Color", 0.0f, 1.0f, 1.0f, 1.0f);
        m_SPoints->Draw(&m_PointsOut[0], m_PointsOut.size());
    }
    void OnImGuiRender() override {}
};

int main(void)
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
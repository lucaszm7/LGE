#pragma once

// Draw Stuff
#include <GL/glew.h>
#include "Geometry.h"
#include "Scene.h"

// Utility
#include "utility/utils.h"



// Queue
std::vector<Vertex> PointsQueue;
std::vector<Vertex> LinesQueue;

void DrawPoint(float x, float y, float r, Color c = { 1.0f, 0.0f, 0.0f, 1.0f })
{
    PointsQueue.emplace_back(x, y, c);
}

void DrawLine(float sx, float sy, float ex, float ey, Color c = { 1.0f, 0.0f, 0.0f, 1.0f })
{
    LinesQueue.emplace_back(sx, sy, c);
    LinesQueue.emplace_back(ex, ey, c);
}

namespace LGE
{


    class Application
    {
    private:
        LGE::Menu* m_MainMenu;
        LGE::Scene_t* m_CurrentApp;

    public:
        // Drawers
        std::unique_ptr<Drawer> DrawerPoints;
        std::unique_ptr<Drawer> DrawerLines;

    public:
        Application()
        {
            Renderer::Init();
            m_CurrentApp = nullptr;
            m_MainMenu = new LGE::Menu(m_CurrentApp);
            m_CurrentApp = m_MainMenu;
            Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);

            DrawerPoints = std::make_unique<Drawer>(SHAPE::POINT);
            DrawerLines = std::make_unique<Drawer>(SHAPE::LINE);
        }

        template <typename T>
        void RegisterScene(const std::string& name)
        {
            std::cout << "Registering Test: " << name << "\n";
            m_MainMenu->m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
        }

        void Run()
        {
            Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);
            while (!Renderer::WindowShouldClose())
            {
                Renderer::Clear();
                Renderer::CreateImGuiFrame();

                /* Render here */
                ImGui::Begin(m_MainMenu->c_SceneName.c_str());
                if (m_CurrentApp != m_MainMenu && ImGui::Button("<- Main Menu"))
                {
                    m_CurrentApp = m_MainMenu;
                    m_MainMenu->c_SceneName = "Main Menu";
                    Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);
                }
                m_CurrentApp->OnUpdate(0.0f);

                // Draw Primitives
                if (LinesQueue.size() > 0)
                {
                    DrawerLines->Draw(&LinesQueue[0], LinesQueue.size());
                    LinesQueue.clear();
                }
                if (PointsQueue.size() > 0)
                {
                    DrawerPoints->Draw(&PointsQueue[0], PointsQueue.size());
                    PointsQueue.clear();
                }

                m_CurrentApp->OnRender();
                m_CurrentApp->OnImGuiRender();
                ImGui::End();
                /* Render Ends */

                Renderer::UpdateImGui();
                Renderer::UpdateGLFW();
            }

            Renderer::CleanUpImGui();
            Renderer::CleanUpGLFW();
        }

    };
}
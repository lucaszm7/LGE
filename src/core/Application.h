#pragma once

// Draw Stuff
#include <GL/glew.h>
#include "Geometry.h"
#include "Scene.h"

// Utility
#include "utility/utils.h"



// Queue
static std::vector<Vertex> PointsQueue;
static std::vector<Vertex> LinesQueue;
static std::vector<Vertex> RectQueue;


void DrawPoint(float x, float y, float r = 50.0f, const Color& c = { 1.0f, 0.0f, 0.0f, 1.0f })
{
    PointsQueue.emplace_back(x, y, c);
    PointsRadius = r;
}

void DrawLine(float sx, float sy, float ex, float ey, const Color& c = { 1.0f, 0.0f, 0.0f, 1.0f })
{
    LinesQueue.emplace_back(sx, sy, c);
    LinesQueue.emplace_back(ex, ey, c);
}

void DrawRect(const glm::vec2& vPos, const glm::vec2& vSize, Color c = { 1.0f, 0.0f, 0.0f, 1.0f })
{
    RectQueue.emplace_back(vPos.x, vPos.y, c);
    RectQueue.emplace_back(vPos.x + vSize.x, vPos.y, c);
    RectQueue.emplace_back(vPos.x + vSize.x, vPos.y + vSize.y, c);
    RectQueue.emplace_back(vPos.x, vPos.y + vSize.y, c);
}

namespace LGE
{
    class TransformedView
    {
    public:
        // ===== Algorithm =====

        // World Offset Offset
        float fOffsetX = 0.0f;
        float fOffsetY = 0.0f;

        // Difference for mouse picking
        float fStartPanX = 0.0f;
        float fStartPanY = 0.0f;

        // Difference for zooming
        float fScaleX = 1.0f;
        float fScaleY = 1.0f;

        bool bHeld = false;

    public:

        void WorldToScreen(float fWorldX, float fWorldY, int& nScreenX, int& nScreenY)
        {
            nScreenX = (int)((fWorldX - fOffsetX) * fScaleX);
            nScreenY = (int)((fWorldY - fOffsetY) * fScaleY);
        }

        void ScreenToWorld(int nScreenX, int nScreenY, float& fWorldX, float& fWorldY)
        {
            fWorldX = ((float)(nScreenX) / fScaleX) + fOffsetX;
            fWorldY = ((float)(nScreenY) / fScaleY) + fOffsetY;
        }

    public:
        TransformedView()
        {
            fOffsetX = -(SCREEN_WIDTH / 2.0f);
            fOffsetY = -(SCREEN_HEIGHT / 2.0f);
        }

        void HandleZoom()
        {
            double fMouseX, fMouseY;
            LGE::GetCursorPos(fMouseX, fMouseY);

            if ((LGE::GetMouseButton() == GLFW_PRESS) && !bHeld)
            {
                bHeld = true;
                fStartPanX = (float)fMouseX;
                fStartPanY = (float)fMouseY;
            }

            if (bHeld)
            {
                fOffsetX -= ((float)fMouseX - fStartPanX) / fScaleX;
                fOffsetY -= ((float)fMouseY - fStartPanY) / fScaleY;

                fStartPanX = (float)fMouseX;
                fStartPanY = (float)fMouseY;
            }

            if ((LGE::GetMouseButton() == GLFW_RELEASE) && bHeld)
            {
                bHeld = false;
                fOffsetX -= ((float)fMouseX - fStartPanX) / fScaleX;
                fOffsetY -= ((float)fMouseY - fStartPanY) / fScaleY;

                fStartPanX = (float)fMouseX;
                fStartPanY = (float)fMouseY;
            }

            float fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom;
            // Now I have the world position of my mouse
            ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom);

            // Zooming with Q and E
            if (LGE::GetKey(GLFW_KEY_E) == GLFW_PRESS)
            {
                fScaleX *= 1.003f;
                fScaleY *= 1.003f;
            }

            if (LGE::GetKey(GLFW_KEY_Q) == GLFW_PRESS)
            {
                fScaleX *= 0.997f;
                fScaleY *= 0.997f;
            }


            // ...now get the location of the cursor in world space again - It will have changed
            // because the scale has changed, but we can offset our world now to fix the zoom
            // location in screen space, because we know how much it changed laterally between
            // the two spatial scales. Neat huh? ;-)
            float fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom;
            ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom);

            fOffsetX += (fMouseWorldX_BeforeZoom - fMouseWorldX_AfterZoom);
            fOffsetY += (fMouseWorldY_BeforeZoom - fMouseWorldY_AfterZoom);

        }

        void Transform(std::vector<Vertex>& ve)
        {
            for (Vertex& p : ve)
            {
                int nScreenX, nScreenY;
                WorldToScreen(p.Position.x, p.Position.y, nScreenX, nScreenY);
                p.Position.x = nScreenX;
                p.Position.y = nScreenY;
            }
        }
    };

}

// Transformed View
LGE::TransformedView tv;

namespace LGE
{

    class Application
    {
    private:
        LGE::Menu* m_MainMenu;
        LGE::Scene_t* m_CurrentApp;

    protected:
        // Drawers
        std::unique_ptr<Drawer> DrawerPoints;
        std::unique_ptr<Drawer> DrawerLines;
        std::unique_ptr<Drawer> DrawerRects;

        // Transform
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Proj;


    public:
        Application()
            : m_Proj(glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f))
        {
            Renderer::Init();
            m_CurrentApp = nullptr;
            m_MainMenu = new LGE::Menu(m_CurrentApp);
            m_CurrentApp = m_MainMenu;
            Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);

            DrawerPoints = std::make_unique<Drawer>(SHAPE::POINT);
            DrawerLines = std::make_unique<Drawer>(SHAPE::LINE);
            DrawerRects = std::make_unique<Drawer>(SHAPE::RECT);

            m_Shader = std::make_unique<Shader>("res/shaders/Basic_2D.shader");
            m_Shader->Bind();
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
                    LinesQueue.clear();
                    PointsQueue.clear();
                    RectQueue.clear();

                    LinesQueue.shrink_to_fit();
                    PointsQueue.shrink_to_fit();
                    RectQueue.shrink_to_fit();
                    
                    DrawerPoints.release();
                    DrawerLines.release();
                    DrawerRects.release();
                    
                    DrawerPoints = std::make_unique<Drawer>(SHAPE::POINT);
                    DrawerLines = std::make_unique<Drawer>(SHAPE::LINE);
                    DrawerRects = std::make_unique<Drawer>(SHAPE::RECT);
                    
                    delete m_CurrentApp;
                    
                    m_CurrentApp = m_MainMenu;
                    m_MainMenu->c_SceneName = "Main Menu";
                    Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);
                }
                tv.HandleZoom();
                PointsRadius *= tv.fScaleX;
                m_CurrentApp->OnUpdate(0.0f);
                // Draw Primitives
                if (LinesQueue.size() > 0)
                {
                    tv.Transform(LinesQueue);
                    DrawerLines->Draw(&LinesQueue[0], LinesQueue.size());
                    LinesQueue.clear();
                }
                if (PointsQueue.size() > 0)
                {
                    tv.Transform(PointsQueue);
                    DrawerPoints->Draw(&PointsQueue[0], PointsQueue.size());
                    PointsQueue.clear();
                }
                
                if (RectQueue.size() > 0)
                {
                    tv.Transform(RectQueue);
                    DrawerRects->Draw(&RectQueue[0], RectQueue.size());
                    RectQueue.clear();
                }

                m_Shader->SetUniformMat4f("u_MVP", m_Proj);
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
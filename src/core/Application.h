#pragma once

// Draw Stuff
#include "GLCore.h"
#include "Geometry.h"
#include "Scene.h"

// Utility
#include "utility/utils.h"

namespace LGE
{
    class Application
    {
    private:
        Renderer renderer;
        LGE::Menu* m_MainMenu;
        LGE::Scene_t* m_CurrentApp;
    public:
        Application()
        {
            
            Renderer::Init(renderer.Window);
            m_CurrentApp = nullptr;
            m_MainMenu = new LGE::Menu(m_CurrentApp);
            m_CurrentApp = m_MainMenu;

            Renderer::ClearColor(0.0f, 0.0f, 0.25f, 1.0f);
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
            while (!Renderer::WindowShouldClose(renderer.Window))
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
                m_CurrentApp->OnRender();
                m_CurrentApp->OnImGuiRender();
                ImGui::End();
                /* Render Ends */

                Renderer::UpdateImGui();
                Renderer::UpdateGLFW(renderer.Window);
            }

            Renderer::CleanUpImGui();
            Renderer::CleanUpGLFW(renderer.Window);
        }

    };
}
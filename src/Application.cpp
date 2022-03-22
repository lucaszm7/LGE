#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLCore.h"

#include "App.h"
#include "TestDemo.h"
#include "TestClearColor.h"



int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the GLFW */
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SandBox Application", NULL, NULL);
    if (!window)
        return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    Renderer::SetupGLEW();
    Renderer::SetupImGui(window);


    App::AppBase* currentApp = nullptr;
    App::AppMenu* mainMenu = new App::AppMenu(currentApp);
    currentApp = mainMenu;

    mainMenu->RegisterApp<App::TestClearColor>("Clear Color");
    mainMenu->RegisterApp<App::TestDemo>("Base Demo");

    /* Loop until the user closes the window */
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
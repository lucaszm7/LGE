#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLCore.h"

#include "tests/Test.h"
#include "tests/TestDemo.h"
#include "tests/TestClearColor.h"



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


    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestDemo>("Base Demo");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
        Renderer::Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Render here */
        if (currentTest)
        {
            ImGui::Begin("Test Menu");
            if (currentTest != testMenu && ImGui::Button("<- Main Menu"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            currentTest->OnImGuiRender();
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

    if (currentTest != testMenu)
        delete testMenu;
    delete currentTest;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
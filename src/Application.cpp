#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define SCREEN_WIDTH 960.0f
#define SCREEN_HEIGHT 540.0f

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

}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SandBox Application", NULL, NULL);
    if (!window)
        return -1;
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
 
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallBack, 0);

    std::cout << "\033[1;32m" << glGetString(GL_VERSION) << "\033[0m" << std::endl;
    
    float positions[] =
    {
        // Vertex attrib
        //   POS     //   UV
        -50.0f, -50.0f, 0.0f, 0.0f, // 0 // This is a vertex
        50.0f, -50.0f, 1.0f, 0.0f, // 1 // Another vertex
        50.0f, 50.0f, 1.0f, 1.0f, // 2 // yet another vertex
        -50.0f, 50.0f, 0.0f, 1.0f, // 3 // and another vertex
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0,
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray VAO;
                           // nº of vertex * size of each vertex
    VertexBuffer vertexbuffer(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    VAO.AddBuffer(vertexbuffer, layout);

    IndexBuffer indexbuffer(indices, 6);

    // Transform to pixel space
    glm::mat4 projOrtho = glm::ortho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f);

    // Camera Transform
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));



    // In debug mode the relative path is the project folder!!!
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();


    Texture texture("res/textures/goblin-12.png");
    texture.Bind();
    // We need to tell our shader, witch texture slot to sample from
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everthing
    VAO.Unbind();
    vertexbuffer.Unbind();
    indexbuffer.Unbind();
    shader.Unbind();

    Renderer renderer;


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


    ImVec4 obj_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glm::vec3 translation1(200.0f, 200.0f, 0.0f);
    glm::vec3 translation2(200.0f, 200.0f, 0.0f);

    glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.Bind();

        {
            // Object Transform = Translation, Rotation, Scale3
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation1);
            glm::mat4 mvp = projOrtho * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.SetUniform4f("u_Color", obj_color.x * obj_color.w, obj_color.y * obj_color.w, obj_color.z * obj_color.w, obj_color.w);
            renderer.Draw(VAO, indexbuffer, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation2);
            glm::mat4 mvp = projOrtho * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.SetUniform4f("u_Color", obj_color.x * obj_color.w, obj_color.y * obj_color.w, obj_color.z * obj_color.w, obj_color.w);
            renderer.Draw(VAO, indexbuffer, shader);
        }

        {
            ImGui::Begin("Tranformations");                          // Create a window called "Hello, world!" and append into it.

            ImGui::SliderFloat3("Translation - 1", &translation1.x, 0.0f, (float)SCREEN_WIDTH);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat3("Translation - 2", &translation2.x, 0.0f, (float)SCREEN_WIDTH);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::NewLine();
            ImGui::ColorEdit3("Obj color", (float*)&obj_color); // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glfwSetKeyCallback(window, key_callback);
        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
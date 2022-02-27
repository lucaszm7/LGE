#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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
            glfwSetWindowMonitor(window, NULL, 200, 200, 640, 480, fullScreenMode->refreshRate);
    }

}



int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "SandBox Application", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


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
        -0.5f, -0.5f, // This is a vertex
         0.5f, -0.5f, // Another vertex
         0.5f,  0.5f, // yet another vertex
        -0.5f,  0.5f,
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0,
    };


    VertexBuffer vertexbuffer(positions, 2 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);

    VertexArray VAO;
    VAO.AddBuffer(vertexbuffer, layout);
        
    IndexBuffer indexbuffer(indices, 6);

    // In debug mode the relative path is the project folder!!!
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
        
    // Unbind everthing
    VAO.Unbind();
    vertexbuffer.Unbind();
    indexbuffer.Unbind();
    shader.Unbind();

    double r = 0.0f, g = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.SetUniform4f("u_Color", r, g, 1.0f, 1.0f);
        VAO.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwGetCursorPos(window, &r, &g);
        r /= 640;
        g /= 480;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        glfwSetKeyCallback(window, key_callback);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
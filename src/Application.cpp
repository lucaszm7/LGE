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

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    if (!stream)
    {
        std::cout << "Wrong shader Path!" << std::endl;
        exit;
    }

    std::stringstream ss[2];

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    std::string line;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)_malloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << 
            " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
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
    
    {
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

        VertexArray VAO;
        VertexBuffer vbuffer(positions, 2 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        VAO.AddBuffer(vbuffer, layout);
        
        IndexBuffer ibo(indices, 6);

        // In debug mode the relative path is the project folder!!!
        ShaderProgramSource shaderSource = ParseShader("res/shaders/Basic.shader");

        unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
        glUseProgram(shader);

        /*
        * Parâmetros de glUniform:
        * Localização do Uniform: Só podemos chamar glUniform depois de termos nosso
        * shader Program, pois quando ele eh compilado, OpenGL reserva uma posição na
        * MEM para aquele Uniform, e precisamos saber qual eh essa localização, então
        * precisamos de um PONTEIRO para aquele objeto, e podemos perguntar ao OpenGL
        * qual a localização desse Uniform pelo nome.
        */
        int UniformColorLocation = glGetUniformLocation(shader, "u_Color");
        if (UniformColorLocation == -1) ASSERT();


        // Unbind everthing
        VAO.Unbind();
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        double r = 0.0f, g = 0.0f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader);
            glUniform4f(UniformColorLocation, r, g, 1.0f, 1.0f);

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
        glDeleteProgram(shader);
    }

    glfwTerminate();
    return 0;
}
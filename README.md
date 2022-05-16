# Lukeda Graphical Engine (LGE)

It's a simple graphical library, written in C++ with OpenGL(GLEW) and GLFW

Start as a fork from my other project named "OpenGL", it grow up when I needed for my class 
"Geometrical Algorithms", and then I decided created this library

## Implementation

Provides an abstraction for creating windows, GUI, and render uneder LGE namespace.
It's necessary create a class inheriting from "LGE::Scene_t", overloading OnUpdate(), OnRender() and OnImGui()
methods.
In main, creates a LGE::Application object, register the Scenes you want with App.RegisterClass<Scene>(name).
And then run the Application App.run();

```c++
#include "Application.h"

class My_Scene : public LGE::Scene_t
{
    public:
    QuadTree(int argc = 0, char** argv = nullptr)
        : m_Proj(glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f))
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic_2D.shader");
        m_Shader->Bind();

        for (int i = 0; i < 10; ++i)
        {
            Point2D p;
            p.Position =
            {
                LGE::rand(0.0f, SCREEN_WIDTH),
                LGE::rand(0.0f, SCREEN_HEIGHT)
            };
            m_PointsIn.push_back(p);
        }

        m_SPoints = std::make_unique<SPoint>(&m_PointsIn[0], m_PointsIn.size());

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
        m_Shader->SetUniformMat4f("u_MVP", m_Proj);
        m_SPoints->Draw();
    }

    void OnImGuiRender() override 
    {

    }

};

int main(int argc, char** argv)
{
    LGE::Application Demo;
    Demo.RegisterScene<My_Scene>("Scene name");
    Demo.Run();

    return 0;
}
```

## DONE:
> OpenGL Abstraction:
    > Vertex Array Objects
    > Vertex Buffer
    > Index Buffer
    > Layouts
    > Shaders
> Scenes
> Geometry:
    > Points
    > Lines
    > Polygons

## TODO:

> Fix Colors:
    > Lines and Polygons should have the option for it's own color
> Finish Line and Polygon Modes
> Separate into different Projects (maybe create another repo called: "2D graphics api"):
    > 2D Graphics API (core): OpenGL stuff and Scenes
    > Geometry API

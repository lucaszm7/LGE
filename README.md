# Lukeda Graphical Engine (LGE)

<img width="642" alt="image" src="https://user-images.githubusercontent.com/42661760/183973013-015fdf78-94dc-4a1c-9ed9-0cee74c643ad.png">
<img width="642" alt="image" src="https://user-images.githubusercontent.com/42661760/183938865-90a91731-5d5c-49b5-adf4-66d8cf175cd7.png">

It's a simple graphical library, written in C++ with OpenGL(GLEW) and GLFW

Start as a fork from my other project named "OpenGL", it grow up when I needed for my class 
"Geometrical Algorithms", and then I decided created this library

Quad-Tree Implementation: https://drive.google.com/file/d/1NzEutmU_LQmYcFGxGamX-hPj5FWaE5nw/view?usp=sharing

## Bugs && Ideias

- Performance affected from fix in memory leak
- Still consuming lots of unknown memory :(

## Implementation

Provides an abstraction for creating windows, GUI, and render uneder LGE namespace.
It's necessary create a class inheriting from `LGE::Scene_t`, overloading `OnUpdate()`, `OnRender()` and `OnImGui()`
methods.
In main, creates a `LGE::Application object`, register the Scenes you want with `App.RegisterClass<Scene>(name)`.
And then run the Application `App.run()`;

```c++
#include "Application.h"

class My_Scene : public LGE::Scene_t
{
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<SPoint> m_SPoints;
    glm::mat4 m_Proj;

    std::vector<Point2D> m_PointsIn;


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

- - - -

## TODO:

- Finish Line and Polygon Modes
- 2D Camera
- 3D Camera
- Abstract `Shapes` and `Shader` into `Application`:
    - In the Scene just call DrawSomething, who handle `Shader` and etc It's the own `Application`
- LGE compile to static lib and it's linked in other projects


## DONE:
- Library 
- OpenGL Abstraction:
    - Vertex Array Objects
    - Vertex Buffer
    - Index Buffer
    - Layouts
    - Shaders
- Scenes Layout
- Geometry:
    - Points
    - Lines
    - Polygons
- Application Architecture

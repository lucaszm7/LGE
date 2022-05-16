// LGE library
#include "Application.h"

// Standart Stuff
#include <iostream>
#include <memory>
#include <vector>

// Example Scenes
#include "TestDemo.h"
#include "TestClearColor.h"
#include "PolygonTest.h"
#include "ConvexHull.h"

struct AABB
{
    glm::vec2 min;
    glm::vec2 max;

    AABB(glm::vec2 min, glm::vec2 max)
    {
        this->min = min;
        this->max = max;
    }
};

struct QuadTree
{
    AABB square;
    QuadTree* nw;
    QuadTree* ne;
    QuadTree* sw;
    QuadTree* se;

    QuadTree(glm::vec2 min, glm::vec2 max)
        : square(min, max) {}

    QuadTree(AABB square)
        : square(square) {}

    void SubDivision()
    {
        nw = new QuadTree(square.min, (square.max / 2.0f));
        ne = new QuadTree(glm::vec2(square.max.x / 2.0f, square.min.y), glm::vec2(square.max.x, square.max.y / 2.0f));
        sw = new QuadTree(glm::vec2(square.min.x, square.max.y / 2.0f), glm::vec2(square.max.x / 2.0f, square.max.y));
        se = new QuadTree((square.max / 2.0f), square.max);
    }

};

class QuadTree_Scene : public LGE::Scene_t
{
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<SPoint> m_SPoints;
    glm::mat4 m_Proj;

    std::vector<Point2D> m_PointsIn;


public:
    QuadTree_Scene(int argc = 0, char** argv = nullptr)
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

    ~QuadTree_Scene()
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

    Demo.RegisterScene<LGE::TestClearColor>("Clear Color Test");
    Demo.RegisterScene<LGE::TestDemo>("Texture Test");
    Demo.RegisterScene<PolygonTest>("Polygon Test");
    Demo.RegisterScene<ConvexHull>("ConvexHull");
    Demo.RegisterScene<QuadTree_Scene>("QuadTree");

    Demo.Run();

    return 0;
}
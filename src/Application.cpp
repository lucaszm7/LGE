#include "Application.h"

// Example Scenes
#include "TestDemo.h"
#include "TestClearColor.h"
#include "PolygonTest.h"
#include "ConvexHull.h"

class QuadTree : public LGE::Scene_t
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
                0.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (SCREEN_WIDTH - 0.0f))),
                0.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (SCREEN_HEIGHT - 0.0f)))
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

    Demo.RegisterScene<LGE::TestClearColor>("Clear Color Test");
    Demo.RegisterScene<LGE::TestDemo>("Texture Test");
    Demo.RegisterScene<PolygonTest>("Polygon Test");
    Demo.RegisterScene<ConvexHull>("ConvexHull");
    Demo.RegisterScene<QuadTree>("QuadTree");

    Demo.Run();

    return 0;
}
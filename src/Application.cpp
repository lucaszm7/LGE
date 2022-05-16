#include "Application.h"


class QuadTree : public LGE::Scene_t
{
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<SPoint> m_SPoints;

public:
    QuadTree(int argc = 0, char** argv = nullptr) 
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_SPoints = std::make_unique<SPoint>();
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
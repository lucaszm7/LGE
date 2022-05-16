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
    // clock-wise, starting up-left
    QuadTree* children[4];

    QuadTree(glm::vec2 min, glm::vec2 max)
        : square(min, max) 
    {
        children[0] = nullptr;
        children[1] = nullptr;
        children[2] = nullptr;
        children[3] = nullptr;
    }

    QuadTree(AABB square)
        : square(square) 
    {
        children[0] = nullptr;
        children[1] = nullptr;
        children[2] = nullptr;
        children[3] = nullptr;
    }

    void SubDivision()
    {
        children[0] = new QuadTree(square.min, (square.max / 2.0f));
        children[1] = new QuadTree(glm::vec2(square.max.x / 2.0f, square.min.y), glm::vec2(square.max.x, square.max.y / 2.0f));
        children[2] = new QuadTree((square.max / 2.0f), square.max);
        children[3] = new QuadTree(glm::vec2(square.min.x, square.max.y / 2.0f), glm::vec2(square.max.x / 2.0f, square.max.y));
    }

    static void Get(QuadTree* root, std::vector<Point2D> &quads)
    {
        quads.emplace_back(root->square.min, glm::vec4(LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), 1.0f));
        quads.emplace_back(root->square.max, glm::vec4(LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), 1.0f));
        std::cout << root->square.min << std::endl;
        std::cout << root->square.max << std::endl;
        QuadTree* aux = root->children[0];
        int i = 1;
        while (aux != nullptr && i < 5)
        {
            Get(aux, quads);
            aux = root->children[i];
            ++i;
        }
    }
};

class QuadTree_Scene : public LGE::Scene_t
{
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<DrawPoint> m_DrawPoints;
    std::unique_ptr<DrawLine> m_DrawLines;
    glm::mat4 m_Proj;

    std::vector<Point> m_Points;
    QuadTree *root;


public:
    QuadTree_Scene(int argc = 0, char** argv = nullptr)
        : m_Proj(glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f))

    {
        root = new QuadTree(glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        m_Shader = std::make_unique<Shader>("res/shaders/Basic_2D.shader");
        m_Shader->Bind();

        for (int i = 0; i < 10; ++i)
        {
            m_Points.emplace_back(LGE::rand(0.0f, SCREEN_WIDTH),
                                  LGE::rand(0.0f, SCREEN_HEIGHT));
        }

        root->SubDivision();

        VertexBufferLayout *layout = new VertexBufferLayout();
        layout->Push<float>(2);
        m_DrawLines = std::make_unique<DrawLine>(&m_Points[0], m_Points.size(), layout, LineType::DEFAULT);
        m_DrawPoints = std::make_unique<DrawPoint>(&m_Points[0], m_Points.size(), layout);

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
        m_DrawPoints->Draw();
        m_DrawLines->Draw();
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
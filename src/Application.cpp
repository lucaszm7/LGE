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
    // Draws
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<DrawPoint> m_DrawPoints;
    std::unique_ptr<DrawLine> m_DrawLines;
    glm::mat4 m_Proj;

    // Algorithm
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

class Test_Zoom : public LGE::Scene_t
{
private:
    // Draws
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<DrawPoint> m_DrawPoints;
    std::unique_ptr<DrawLine> m_DrawLines;
    glm::mat4 m_Proj;

    // Algorithm
    std::vector<Point> m_Points;
    float fOffsetX = 0.0f;
    float fOffsetY = 0.0f;
    
    float fStartPanX = 0.0f;
    float fStartPanY = 0.0f;

    bool bHeld = false;

protected:

    void WorldToScreen(float fWorldX, float fWorldY, int& nScreenX, int& nScreenY)
    {
        nScreenX = (int)(fWorldX - fOffsetX);
        nScreenY = (int)(fWorldY - fOffsetY);
    }
    
    void ScreenToWorld(int nScreenX, int nScreenY, float &fWorldX, float &fWorldY)
    {
        fWorldX = (float)(nScreenX) + fOffsetX;
        fWorldY = (float)(nScreenY) + fOffsetY;
    }

public:
    Test_Zoom(int argc = 0, char** argv = nullptr)
        : m_Proj(glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f))
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic_2D.shader");
        m_Shader->Bind();

        VertexBufferLayout* layout = new VertexBufferLayout();
        layout->Push<float>(2);
        m_DrawLines = std::make_unique<DrawLine>(nullptr, 1024, layout);
        m_DrawPoints = std::make_unique<DrawPoint>();

        fOffsetX = -(SCREEN_WIDTH / 2.0f);
        fOffsetY = -(SCREEN_HEIGHT / 2.0f);

         

    }

    ~Test_Zoom()
    {
        m_Shader->Unbind();
    }

    void OnUpdate(float deltaTime) override
    {

        double fMouseX, fMouseY;
        LGE::GetCursorPos(fMouseX, fMouseY);

        if ((LGE::GetMouseButton() == GLFW_PRESS) && !bHeld)
        {
            bHeld = true;
            fStartPanX = fMouseX;
            fStartPanY = fMouseY;
        }

        if (bHeld)
        {
            fOffsetX -= (fMouseX - fStartPanX);
            fOffsetY -= (fMouseY - fStartPanY);

            fStartPanX = fMouseX;
            fStartPanY = fMouseY;
        }

        if ((LGE::GetMouseButton() == GLFW_RELEASE) && bHeld)
        {
            bHeld = false;
            fOffsetX -= (fMouseX - fStartPanX);
            fOffsetY -= (fMouseY - fStartPanY);
        }


        m_Points.clear();
        // Draw 10 Horizontal Lines
        for (float y = 0.0f; y < 100.0f; y++)
        {
            float sx = 0.0f, sy = y;
            float ex = 100.0f, ey = y;

            int pixel_sx, pixel_sy, pixel_ex, pixel_ey;

            WorldToScreen(sx, sy, pixel_sx, pixel_sy);
            WorldToScreen(ex, ey, pixel_ex, pixel_ey);

            m_Points.emplace_back(pixel_sx, pixel_sy);
            m_Points.emplace_back(pixel_ex, pixel_ey);
        }
        
        // Draw 10 Vertical Lines
        for (float x = 0.0f; x < 100.0f; x++)
        {
            float sx = x, sy = 0.0f;
            float ex = x, ey = 100.0f;

            int pixel_sx, pixel_sy, pixel_ex, pixel_ey;

            WorldToScreen(sx, sy, pixel_sx, pixel_sy);
            WorldToScreen(ex, ey, pixel_ex, pixel_ey);

            m_Points.emplace_back(pixel_sx, pixel_sy);
            m_Points.emplace_back(pixel_ex, pixel_ey);
        }
    }

    void OnRender() override
    {
        m_Shader->SetUniformMat4f("u_MVP", m_Proj);
        m_DrawLines->Draw(&m_Points[0], m_Points.size());
        // m_DrawLines->Draw();
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
    Demo.RegisterScene<Test_Zoom>("Test Zooming");

    Demo.Run();

    return 0;
}
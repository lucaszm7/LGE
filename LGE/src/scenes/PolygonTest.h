#pragma once

#include "Scene.h"
#include "Geometry.h"

class PolygonTest : public LGE::Scene_t
{
private:

    std::unique_ptr<Shader> m_Shader;

    std::unique_ptr<DrawPoint> m_DrawPoints;
    std::unique_ptr<DrawLine> m_DrawLines;
    std::unique_ptr<SPolygon> m_SPolygon;
    std::vector<Point2D> m_Points2D;

public:
    PolygonTest()
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

        srand((unsigned int)time(0));
        for (int i = 0; i < 50; ++i)
        {
            Point2D p;
            p.Position =
            {
                -1.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f))),
                -1.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)))
            };

            p.Color =
            {
                0.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.0f))),
                0.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.0f))),
                0.0f + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.0f))),
                1.0f
            };

            m_Points2D.push_back(p);
        }


        m_DrawPoints = std::make_unique<DrawPoint>(&m_Points2D[0], m_Points2D.size());
        m_DrawLines = std::make_unique<DrawLine>(&m_Points2D[0], m_Points2D.size());
        m_SPolygon = std::make_unique<SPolygon>(&m_Points2D[0], m_Points2D.size());
    }

    ~PolygonTest()
    {
        m_Shader->Unbind();
    }

    void OnUpdate(float deltaTime) override
    {

    }
    void OnRender() override
    {
        m_Shader->Bind();

        m_Shader->SetUniform1i("u_color", 1);
        m_Shader->SetUniform4f("u_Color", 0.0f, 1.0f, 1.0f, 1.0f);
        m_SPolygon->Draw(&m_Points2D[0]);
        m_Shader->SetUniform1i("u_color", 0);

        m_DrawPoints->Draw(&m_Points2D[0], m_Points2D.size());
        m_Shader->SetUniform1i("u_color", 1);
        m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);
        m_DrawLines->Draw(&m_Points2D[0], m_Points2D.size());
        m_Shader->SetUniform1i("u_color", 0);

    }
    void OnImGuiRender() override
    {
    }
};
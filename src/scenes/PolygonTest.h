#pragma once

#include "Scene.h"
#include "GLCore.h"
#include "Geometry.h"

class PolygonTest : public LGE::Scene_t
{
private:

    std::unique_ptr<Shader> m_Shader;

    std::unique_ptr<SPoint> m_SPoints;
    std::unique_ptr<SLine> m_SLines;
    std::unique_ptr<SPolygon> m_SPolygon;
    std::vector<Point2D> m_Points2D;

public:
    PolygonTest()
    {
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

        srand(time(0));
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


        m_SPoints = std::make_unique<SPoint>(&m_Points2D[0], m_Points2D.size());
        m_SLines = std::make_unique<SLine>(&m_Points2D[0], m_Points2D.size());
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
        m_SPolygon->Draw();
        m_Shader->SetUniform1i("u_color", 0);

        m_SPoints->Draw();
        m_Shader->SetUniform1i("u_color", 1);
        m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);
        m_SLines->Draw();
        m_Shader->SetUniform1i("u_color", 0);

    }
    void OnImGuiRender() override
    {
    }
};
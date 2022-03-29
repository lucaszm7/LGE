#pragma once

#include "GLCore.h"
#include <memory>
#include <array>
#include <vector>


struct Vertex
{
    glm::vec2 Position = { 0.0f, 0.0f };
    glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };;
};

using Point2D = Vertex;

struct Shape
{
    virtual void Draw() {};
};

struct SPoint : Shape
{
public:
    Vertex* vertex;
    unsigned int vertex_size;
    float radius;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<IndexBuffer> m_IB;

public:
    SPoint(Point2D* p = nullptr, unsigned int v_size = 1, float r = 50)
    {
        vertex = p;
        vertex_size = v_size;
        radius = r;

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * vertex_size, GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        m_VAO->AddBuffer(*m_VB, layout);

        unsigned int *index = new unsigned int[vertex_size];
        for (int i = 0; i < vertex_size; ++i)
        {
            index[i] = i;
        }
        m_IB = std::make_unique<IndexBuffer>(index, vertex_size);
    }

    ~SPoint()
    {
        m_VB->Unbind();
        m_VAO->Unbind();
    }

    void Draw(Point2D* pArray = nullptr)
    {
        if (pArray)
            vertex = pArray;
        if (!vertex)
            std::cerr << "Drawing nullptr.\n";

        m_VB->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(*vertex) * vertex_size, &(*vertex));
        m_VAO->Bind();
        glEnable(GL_POINT_SMOOTH);
        glPointSize(radius);
        glDrawElements(GL_POINTS, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
};

struct Line : Shape
{
public:
    std::vector<Vertex> vertex;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<IndexBuffer> m_IB;

public:
    Line()
    {
        CreateLine();
        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 12, GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        m_VAO->AddBuffer(*m_VB, layout);
        std::vector<unsigned int> index;
        for (int i = 0; i < vertex.size(); ++i)
        {
            index.push_back(i);
        }

        m_IB = std::make_unique<IndexBuffer>(&index[0], index.size());
    }

    ~Line()
    {
        m_VB->Unbind();
        m_VAO->Unbind();
    }

    void CreateLine(glm::vec4 color = { 0.5f, 0.5f ,0.0f,1.0f })
    {
        float size = 1.0f;

        Vertex v0;
        v0.Position.x = 0.0f;
        v0.Position.y = 0.0f;
        v0.Color.x = color.x;
        v0.Color.y = color.y;
        v0.Color.z = color.z;
        v0.Color.w = color.w;

        vertex.push_back(v0);

        Vertex v1;
        v1.Position.x = 1.0f;
        v1.Position.y = 1.0f;
        v1.Color.x = color.x;
        v1.Color.y = color.y;
        v1.Color.z = color.z;
        v1.Color.w = color.w;

        vertex.push_back(v1);

    }

    void Set(glm::vec4 c = { 0.0f, 1.0f ,0.0f,1.0f })
    {
        CreateLine();
    }

    void Draw()
    {
        m_VB->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex.size(), &vertex[0]);
        m_VAO->Bind();
        glDrawElements(GL_LINE_LOOP, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
};

struct Tri : Shape
{
public:
    std::array<Vertex, 3> vertex;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<IndexBuffer> m_IB;

public:
    Tri(float x = 0.0f, float y = 0.0f)
    {
        vertex = CreateTri(x, y);

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 3, GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        m_VAO->AddBuffer(*m_VB, layout);
        unsigned int index[] =
        {
            0, 1, 2,
        };
        m_IB = std::make_unique<IndexBuffer>(index, 3);
    }

    ~Tri()
    {
        m_VB->Unbind();
        m_VAO->Unbind();
    }

    static std::array<Vertex, 3> CreateTri(float x, float y, glm::vec4 color = { 0.0f, 1.0f ,0.0f,1.0f })
    {
        float size = 1.0f;

        Vertex v0;
        v0.Position.x = x;
        v0.Position.y = y;
        v0.Color.x = color.x;
        v0.Color.y = color.y;
        v0.Color.z = color.z;
        v0.Color.w = color.w;

        Vertex v1;
        v1.Position.x = x + size;
        v1.Position.y = y;
        v1.Color.x = color.x;
        v1.Color.y = color.y;
        v1.Color.z = color.z;
        v1.Color.w = color.w;

        Vertex v2;
        v2.Position.x = x + size;
        v2.Position.y = y + size;
        v2.Color.x = color.x;
        v2.Color.y = color.y;
        v2.Color.z = color.z;
        v2.Color.w = color.w;


        return { v0, v1, v2 };
    }

    void Set(float x, float y, glm::vec4 c = { 0.0f, 1.0f ,0.0f,1.0f })
    {
        vertex = CreateTri(x, y, c);
    }

    void Draw()
    {
        Vertex vertices[3];
        memcpy(vertices, vertex.data(), vertex.size() * sizeof(Vertex));
        m_VB->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
};

struct Quad : Shape
{
public:
    std::array<Vertex, 4> vertex;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<IndexBuffer> m_IB;

public:
    Quad(float x = 0.0f, float y = 0.0f)
    {
        vertex = CreateQuad(x, y);

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 4, GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        m_VAO->AddBuffer(*m_VB, layout);
        unsigned int index[] =
        {
            0, 1, 2,
            2, 3, 0
        };
        m_IB = std::make_unique<IndexBuffer>(index, 6);
    }

    ~Quad()
    {
        m_VB->Unbind();
        m_VAO->Unbind();
    }

    static std::array<Vertex, 4> CreateQuad(float x, float y)
    {
        float size = 1.0f;

        Vertex v0;
        v0.Position.x = x;
        v0.Position.y = y;
        v0.Color.x = 0.0f;
        v0.Color.y = 0.0f;
        v0.Color.z = 1.0f;
        v0.Color.w = 1.0f;

        Vertex v1;
        v1.Position.x = x + size;
        v1.Position.y = y;
        v1.Color.x = 0.0f;
        v1.Color.y = 0.0f;
        v1.Color.z = 1.0f;
        v1.Color.w = 1.0f;

        Vertex v2;
        v2.Position.x = x + size;
        v2.Position.y = y + size;
        v2.Color.x = 0.0f;
        v2.Color.y = 0.0f;
        v2.Color.z = 1.0f;
        v2.Color.w = 1.0f;

        Vertex v3;
        v3.Position.x = x;
        v3.Position.y = y + size;
        v3.Color.x = 0.0f;
        v3.Color.y = 0.0f;
        v3.Color.z = 1.0f;
        v3.Color.w = 1.0f;

        return { v0, v1, v2, v3 };
    }

    void Set(float x, float y)
    {
        vertex = CreateQuad(x, y);
    }

    void Draw()
    {
        Vertex vertices[4];
        memcpy(vertices, vertex.data(), vertex.size() * sizeof(Vertex));
        m_VB->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        m_VAO->Bind();
        glDrawElements(GL_POLYGON, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
};

struct Polygon : Shape
{

};
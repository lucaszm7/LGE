#pragma once

#include <iostream>
#include <memory>
#include <array>
#include <vector>

#include "GLCore.h"


struct Vertex
{
    glm::vec2 Position;
    glm::vec4 Color;

    Vertex()
    {
        Position = { 0.0f, 0.0f };
        Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    }

    Vertex(float x, float y)
    {
        glm::vec2 p(x, y);
        Position = p;
        Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    }
    
    Vertex(glm::vec2 p)
    {
        Position = p;
        Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    }

    Vertex(glm::vec2 p, glm::vec4 c)
    {
        Position = p;
        Color = c;
    }

    bool operator <(const Vertex& p) const {
        return Position.x < p.Position.x || (Position.x == p.Position.x && Position.y < p.Position.y);
    }

};

using Point2D = Vertex;

inline std::ostream& operator << (std::ostream& out, Vertex v)
{
    out << v.Position.x << ", " << v.Position.y;
    return out;
}

struct Shape
{
    virtual void Draw() {};
};

// S = Shape
struct SPoint : Shape
{
public:
    Vertex* vertex;
    unsigned int vertex_size;
    float radius;

private:
    std::vector<unsigned int> m_Index;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<IndexBuffer> m_IB;

public:
    SPoint(Point2D* p = nullptr, unsigned int v_size = 10, float r = 50)
    {
        vertex = p;
        vertex_size = v_size;
        radius = r;

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(vertex, sizeof(Vertex) * vertex_size, GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        m_VAO->AddBuffer(*m_VB, layout);

        m_Index.resize(vertex_size);
        for (int i = 0; i < vertex_size; ++i)
        {
            m_Index[i] = i;
        }
        m_IB = std::make_unique<IndexBuffer>(&m_Index[0], vertex_size);
    }

    ~SPoint()
    {
        m_VB->Unbind();
        m_VAO->Unbind();
    }

    void Draw(Point2D* pArray = nullptr, size_t v_size = 0)
    {
        if (pArray && v_size != 0)
        {
            vertex = pArray;
            if (v_size > m_Index.size())
            {
                m_Index.resize(v_size);
                for (int i = 0; i < v_size; ++i)
                    m_Index[i] = i;
                m_IB.release();
                m_IB = std::make_unique<IndexBuffer>(&m_Index[0], m_Index.size());

                m_VB.release();
                m_VB = std::make_unique<VertexBuffer>(vertex, sizeof(Vertex) * v_size, GL_DYNAMIC_DRAW);
                VertexBufferLayout layout;
                layout.Push<float>(2);
                layout.Push<float>(4);
                m_VAO->AddBuffer(*m_VB, layout);
            }
            vertex_size = v_size;
        }

        if (!vertex)
            std::cerr << "Drawing nullptr.\n";

        m_VAO->Bind();
        m_VB->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex_size, &(*vertex));
        m_IB->Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * vertex_size, &(m_Index[0]));
        m_VAO->Bind();
        glEnable(GL_POINT_SMOOTH);
        glPointSize(radius);
        glDrawElements(GL_POINTS, vertex_size, GL_UNSIGNED_INT, nullptr);
    }
};

// S = Shape
struct SLine : Shape
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
    SLine(Point2D* p = nullptr, unsigned int v_size = 1, float r = 50)
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

        unsigned int* index = new unsigned int[vertex_size];
        for (int i = 0; i < vertex_size; ++i)
        {
            index[i] = i;
        }
        m_IB = std::make_unique<IndexBuffer>(index, vertex_size);
    }

    ~SLine()
    {
        m_VB->Unbind();
        m_VAO->Unbind();
    }

    void Draw(GLenum mode = GL_LINE_STRIP, Point2D* pArray = nullptr)
    {
        if (pArray)
            vertex = pArray;
        if (!vertex)
            std::cerr << "Drawing nullptr.\n";

        m_VB->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex_size, &(*vertex));
        m_VAO->Bind();
        glDrawElements(mode, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
};

// S = Shape
struct SPolygon : Shape
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
    SPolygon(Point2D* p = nullptr, unsigned int v_size = 1, float r = 50)
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

        unsigned int* index = new unsigned int[vertex_size];
        for (int i = 0; i < vertex_size; ++i)
        {
            index[i] = i;
        }
        m_IB = std::make_unique<IndexBuffer>(index, vertex_size);
    }

    ~SPolygon()
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex_size, &(*vertex));
        m_VAO->Bind();
        glDrawElements(GL_POLYGON, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
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
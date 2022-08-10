#pragma once
#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include "Renderer.h"


using Color = glm::vec4;

struct Point
{
    float x, y;
    Point(glm::vec2 v)
        :x(v.x), y(v.y) {}
    Point(float x, float y)
        :x(x), y(y) {}
};

struct Line
{
    Point s, e;
};

struct Vertex
{
    glm::vec2 Position;
    glm::vec4 Color;

    Vertex()
    {
        Position = { 0.0f, 0.0f };
        Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    }

    Vertex(const Vertex& a)
        : Position(a.Position), Color(a.Color) {}

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
    
    Vertex(Point p)
    {
        Position.x = p.x;
        Position.y = p.y;
        Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    }

    Vertex(glm::vec2 p, glm::vec4 c)
    {
        Position = p;
        Color = c;
    }

    Vertex(float x, float y, glm::vec4 c)
    {
        glm::vec2 p(x, y);
        Position = p;
        Color = c;
    }
    
    Vertex(Point p, glm::vec4 c)
    {
        Position.x = p.x;
        Position.y = p.y;
        this->Color = c;
    }

    bool operator < (const Vertex& p) const {
        return Position.x < p.Position.x || (Position.x == p.Position.x && Position.y < p.Position.y);
    }

};
using Point2D = Vertex;

inline std::ostream& operator << (std::ostream& out, const Vertex& v)
{
    out << v.Position.x << ", " << v.Position.y;
    return out;
}


enum class SHAPE
{
    POINT = GL_POINTS,
    LINE = GL_LINES,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_LOOP = GL_LINE_LOOP,
    RECT = GL_TRIANGLES,

};
struct Drawer
{
public:
    void* dta;
    size_t dta_size;
    SHAPE type;

private:
    std::vector<unsigned int> m_Index;
    VertexBufferLayout *m_Layout;

    static const unsigned int nBufferMaxSize = 500000;

private:
    std::vector<VertexArray> m_VAO;
    std::vector<VertexBuffer> m_VB;
    std::vector<IndexBuffer> m_IB;

public:
    Drawer(SHAPE t, size_t v_size = nBufferMaxSize, void* pdta = nullptr, VertexBufferLayout *layout = nullptr, float r = 50)
    {
        glPointSize(r);
        m_VAO.reserve(100);
        m_VB.reserve(100);
        m_IB.reserve(100);
        dta = pdta;
        dta_size = v_size;
        type = t;

        m_VAO.emplace_back();
        if (layout)
            m_Layout = layout;

        else
        {
            m_Layout = new VertexBufferLayout();
            m_Layout->Push<float>(2);
            m_Layout->Push<float>(4);
        }
        m_VB.emplace_back(dta, m_Layout->GetStride() * dta_size, GL_DYNAMIC_DRAW);
        m_VAO[0].AddBuffer(m_VB[0], *m_Layout);


        if (static_cast<SHAPE>(type) == SHAPE::RECT)
        {
            m_Index.resize(v_size * (3.0f / 2.0f));
            for (int i = 0; i < m_Index.size() * (2.0f / 3.0f); i += 4)
            {
                m_Index[(i + 0) + (i / 2)] = i + 0;
                m_Index[(i + 1) + (i / 2)] = i + 1;
                m_Index[(i + 2) + (i / 2)] = i + 2;
                                    
                m_Index[(i + 3) + (i / 2)] = i + 0;
                m_Index[(i + 4) + (i / 2)] = i + 2;
                m_Index[(i + 5) + (i / 2)] = i + 3;
            }
        }

        else {
            m_Index.resize(dta_size);
            for (unsigned int i = 0; i < dta_size; ++i)
            {
                m_Index[i] = i;
            }
        }
        m_IB.emplace_back(nullptr, m_Index.size());
    }

    ~Drawer()
    {
        dta = nullptr;
        dta_size = 0;
        m_VAO.clear();
        m_VB.clear();
        m_IB.clear();
        m_Index.clear();
        delete m_Layout;
    }

    void Reset()
    {
        dta = nullptr;
        dta_size = 0;
        m_VAO.clear();
        m_VB.clear();
        m_IB.clear();
        m_Index.clear();
    }

    void Draw(void* pdta = nullptr, size_t v_size = 0)
    {
        if (pdta != nullptr && v_size != 0)
        {
            dta = pdta;
            while ((float)(nBufferMaxSize * m_VB.size()) / (float)v_size < 1.0)
            {
                if (static_cast<SHAPE>(type) == SHAPE::RECT)
                {
                    m_Index.resize((m_VAO.size() + 1) * nBufferMaxSize * (3.0f / 2.0f));
                    for (int i = 0; i < m_Index.size() * (2.0f / 3.0f); i += 4)
                    {
                        m_Index[(i + 0) + (i / 2)] = i + 0;
                        m_Index[(i + 1) + (i / 2)] = i + 1;
                        m_Index[(i + 2) + (i / 2)] = i + 2;

                        m_Index[(i + 3) + (i / 2)] = i + 0;
                        m_Index[(i + 4) + (i / 2)] = i + 2;
                        m_Index[(i + 5) + (i / 2)] = i + 3;
                    }
                    m_VB.emplace_back(nullptr, sizeof(Vertex) * nBufferMaxSize);
                    m_IB.emplace_back(nullptr, nBufferMaxSize* (3.0f / 2.0f));
                    m_VAO.emplace_back();
                    m_VAO[m_VAO.size() - 1].AddBuffer(m_VB[m_VB.size()-1], *m_Layout);
                }
                else
                {
                    m_Index.resize((m_VAO.size() + 1) * nBufferMaxSize);
                    for (unsigned int i = 0; i < m_Index.size(); ++i)
                        m_Index[i] = i;
                    m_VB.emplace_back(nullptr, sizeof(Vertex) * nBufferMaxSize);
                    m_IB.emplace_back(nullptr, nBufferMaxSize);
                    m_VAO.emplace_back();
                    m_VAO[m_VAO.size() - 1].AddBuffer(m_VB[m_VB.size() - 1], *m_Layout);
                }
            }
            dta_size = v_size;
        }

        if (!dta)
            return;

        // Difference
        if (static_cast<SHAPE>(type) == SHAPE::POINT)
        {
            glEnable(GL_POINT_SMOOTH);
            // glPointSize(PointsRadius);
            // TODO - Pass a buffer to the shader with radius info
        }

        size_t dta_draw = nBufferMaxSize;
        for (int i = 0; i <= (dta_size / nBufferMaxSize); ++i)
        {
            if (i == dta_size / nBufferMaxSize) dta_draw = (dta_size % nBufferMaxSize);

            m_VB[i].Bind();
            void* dta_location = (void*)(((char*)dta) + (nBufferMaxSize * i * m_Layout->GetStride()));
            glBufferSubData(GL_ARRAY_BUFFER, 0, m_Layout->GetStride() * dta_draw, dta_location);

            m_IB[i].Bind();
            size_t index_data_size; 
            size_t index_data_pointer; 
            if (static_cast<SHAPE>(type) == SHAPE::RECT)
            {
                index_data_size = ((float)dta_draw * (3.0f / 2.0f));
                index_data_pointer = nBufferMaxSize * (3.0f / 2.0f) * i; // wront for the last one
            }
            else
            {
                index_data_size = dta_draw;
                index_data_pointer = nBufferMaxSize * i; // wront for the last one
            }
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * index_data_size, &(m_Index[index_data_pointer]));

            m_VAO[i].Bind();

            glDrawElements(static_cast<GLenum>(type), index_data_size, GL_UNSIGNED_INT, nullptr);

            m_VAO[i].Unbind();
            m_IB[i].Unbind();
            m_VB[i].Unbind();
        }

    }
};

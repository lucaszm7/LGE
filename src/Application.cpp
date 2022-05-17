// LGE library
#include "Application.h"

// Standart Stuff
#include <iostream>
#include <memory>
#include <vector>

struct rect
{
    glm::vec2 pos;
    glm::vec2 size;

    rect(const glm::vec2& p = {0.0f, 0.0f}, const glm::vec2& s = {1.0f, 1.0f})
        : pos(p), size(s) {}

    constexpr bool contains(const glm::vec2& p) const
    {
        return !(p.x < pos.x || p.y < pos.y || p.x >= (pos.x + size.x) || p.y >= (pos.y + size.y));
    }

    // Fully contains another rect
    constexpr bool contains(const rect& r) const
    {
        return (r.pos.x >= pos.x) && ((r.pos.x + r.size.x) < (pos.x + size.x)) &&
            (r.pos.y >= pos.y) && ((r.pos.y + r.size.y) < (pos.y + size.y));
    }

    constexpr bool overlaps(const rect& r) const
    {
        return (pos.x < r.pos.x + r.size.x) && (pos.x + size.x >= r.pos.x) &&
            (pos.y < r.pos.y + r.size.y) && (pos.y + size.y >= r.pos.y);
    }

};

class StaticQuadTree : public LGE::Scene_t
{

protected:
    struct SomeObjectWithArea
    {
        glm::vec2 vPos;
        glm::vec2 vVel;
        glm::vec2 vSize;
        Color col;
    };

    std::vector<SomeObjectWithArea> vecObjects;

    float fArea = 100000.0f;

public:
    StaticQuadTree()
    {
        for (int i = 0; i < 100000; ++i)
        {
            SomeObjectWithArea obj;
            obj.vPos = { LGE::rand(0.0f, fArea), LGE::rand(0.0f, fArea) };
            obj.vSize = { LGE::rand(0.1f, 100.0f), LGE::rand(0.1f, 100.0f) };
            obj.col = { LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), 1.0f };
            vecObjects.push_back(obj);
        }
        SomeObjectWithArea obj;
        obj.vPos = { 0.0f, 0.0f };
        obj.vSize = { 100.0f, 100.0f };
        obj.col = { 1.0f, 0.0f, 0.0f, 1.0f };
        vecObjects.push_back(obj);

    }

    void OnUpdate(float fElapsedTime) override
    {
        float fWorldTLX, fWorldTLY;
        float fWorldBRX, fWorldBRY;

        tv.ScreenToWorld(0, 0, fWorldTLX, fWorldTLY);
        tv.ScreenToWorld(SCREEN_WIDTH, SCREEN_HEIGHT, fWorldBRX, fWorldBRY);

        glm::vec2 vWorldTL = { fWorldTLX, fWorldTLY };
        glm::vec2 vWorldBR = { fWorldBRX, fWorldBRY };

        rect rScreen = { vWorldTL, vWorldBR - vWorldTL };

        for (const auto& obj : vecObjects)
        {
            if (rScreen.overlaps({ obj.vPos, obj.vSize }))
            {
                DrawRect(obj.vPos, obj.vSize, obj.col);
            }
        }

    }

    void OnRender() override
    {

    }

    void OnImGuiRender() override
    {

    }

    ~StaticQuadTree()
    {

    }
};

int main(int argc, char** argv)
{
    LGE::Application Demo;
    Demo.RegisterScene<StaticQuadTree>("Static Quad Tree");
    Demo.Run();

    /*Demo.RegisterScene<LGE::TestClearColor>("Clear Color Test");
    Demo.RegisterScene<LGE::TestDemo>("Texture Test");
    Demo.RegisterScene<PolygonTest>("Polygon Test");
    Demo.RegisterScene<ConvexHull>("ConvexHull");
    Demo.RegisterScene<QuadTree_Scene>("QuadTree");*/

    return 0;
}
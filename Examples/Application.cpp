#include "Application.h"

// Standart Stuff
#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <list>
#include <deque>


struct rect
{
    glm::vec2 pos;
    glm::vec2 size;

    rect(const glm::vec2& p = {0.0f, 0.0f}, const glm::vec2& s = {1.0f, 1.0f})
        : pos(p), size(s) {}

    constexpr bool contains(const glm::vec2& p) const
    {
        return !(p.x < pos.x || 
                 p.y < pos.y || 
                 p.x >= (pos.x + size.x) || 
                 p.y >= (pos.y + size.y));
    }

    // Fully contains another rect
    constexpr bool contains(const rect& r) const
    {
        return (r.pos.x >= pos.x) && 
               ((r.pos.x + r.size.x) < (pos.x + size.x)) &&
               (r.pos.y >= pos.y) && 
               ((r.pos.y + r.size.y) < (pos.y + size.y));
    }

    constexpr bool overlaps(const rect& r) const
    {
        return (pos.x < r.pos.x + r.size.x) && (pos.x + size.x >= r.pos.x) &&
            (pos.y < r.pos.y + r.size.y) && (pos.y + size.y >= r.pos.y);
    }

};

constexpr size_t MAX_DEPTH = 32;

template <typename OBJECT_TYPE>
class StaticQuadTree
{

protected:

    size_t m_Depth = 0;

    // This QuadTree
    rect m_Rect;

    // 4 Child Areas
    std::array<rect, 4> m_rChild{};

    // 4 Potential Children
    std::array<std::shared_ptr<StaticQuadTree<OBJECT_TYPE>>, 4> m_pChild {};

    // Item Storage
    std::vector<std::pair<rect, OBJECT_TYPE>> m_pItems;


public:
    StaticQuadTree(const rect& size = { {0.0f, 0.0f},{100.0f, 100.0f} }, const size_t depth = 0)
    {
        m_Depth = depth;
        resize(size);
    }

    void resize(const rect& r)
    {
        clear();
        m_Rect = r;
        glm::vec2 vChildSize = (m_Rect.size / 2.0f);

        m_rChild =
        {
            rect(m_Rect.pos, vChildSize),
            rect(glm::vec2( m_Rect.pos.x + vChildSize.x, m_Rect.pos.y ), vChildSize),
            rect(glm::vec2( m_Rect.pos.x, m_Rect.pos.y + vChildSize.y ), vChildSize),
            rect(m_Rect.pos + vChildSize, vChildSize)
        };
    }

    void clear()
    {
        m_pItems.clear();
        for (int i = 0; i < 4; ++i)
        {
            if (m_pChild[i])
                m_pChild[i]->clear();
        }
    }

    size_t size()
    {
        size_t nCount = m_pItems.size();
        for (int i = 0; i < 4; ++i)
        {
            if (m_pChild[i])
                nCount += m_pChild->size();
        }
        return nCount;
    }

    void insert(const OBJECT_TYPE& item, const rect& itemSize) // AABB
    {
        for (int i = 0; i < 4; ++i)
        {
            if (m_rChild[i].contains(itemSize))
            {
                if (m_Depth + 1 < MAX_DEPTH)
                {
                    if (!m_pChild[i])
                    {
                        m_pChild[i] = std::make_shared<StaticQuadTree<OBJECT_TYPE>>(m_rChild[i], m_Depth + 1);
                    }
                    m_pChild[i]->insert(item, itemSize);
                    return;
                }
            }
        }
        m_pItems.push_back({ itemSize, item });
    }


    /*const std::vector<OBJECT_TYPE>& search(rect& rArea) const
    {
        std::vector<OBJECT_TYPE> listItems;
        listItems.reserve(100000)
        listItems.resize(10);
        search(rArea, listItems);
        return listItems;
    }*/

    void search(const rect& rArea, std::vector<OBJECT_TYPE>& listItems) const
    {
        // First gonna check if the area belongs to this area
        for (const auto& p : m_pItems)
        {
            if (rArea.overlaps(p.first))
                listItems.push_back(p.second);
        }

        // Second we recursivily iterate for the childrens
        #pragma unroll
        for (int i = 0; i < 4; ++i)
        {
            if (m_pChild[i])
            {
                // if the children area enteryly belongs to search area, add whole children
                if (rArea.contains(m_rChild[i]))
                    m_pChild[i]->items(listItems);

                // Now if its not fully contained, its need to be searched
                else if (m_rChild[i].overlaps(rArea))
                    m_pChild[i]->search(rArea, listItems);
            }
        }
    }

    void items(std::vector<OBJECT_TYPE>& listItems) const
    {
        for (const auto& p : m_pItems)
        {
            listItems.push_back(p.second);
        }

        for (int i = 0; i < 4; ++i)
        {
            if (m_pChild[i])
                m_pChild[i]->items(listItems);
        }

    }

    const rect& area()
    {
        return m_Rect;
    }

    const void Draw() const
    {
        DrawRectEmpty(m_Rect.pos, m_Rect.size, Color(1.0f, 1.0f, 0.0f, 1.0f));
        for (unsigned int i = 0; i < 4; ++i)
        {
            if (m_pChild[i])
                m_pChild[i]->Draw();
        }

    }

};


template<typename OBJECT_TYPE>
class StaticQuadTreeContainer
{
    using QuadTreeContainer = std::list<OBJECT_TYPE>;

protected:
    QuadTreeContainer m_AllItems;

    StaticQuadTree<typename QuadTreeContainer::iterator> root;
    mutable std::vector<typename QuadTreeContainer::iterator> listItemPointers;

public:
    StaticQuadTreeContainer(const rect& size = { {0.0f, 0.0f},{100.0f, 100.0f} }, const size_t depth = 0)
        : root(size) {
        listItemPointers.reserve(100001);
    }

    void resize(const rect& area)
    {
        root.resize(area);
    }

    size_t size() const
    {
        return m_AllItems.size();
    }

    bool empty() const
    {
        return m_AllItems.empty();
    }

    void clear()
    {
        root.clear();
        m_AllItems.clear();
    }

    typename QuadTreeContainer::iterator begin()
    {
        return m_AllItems.begin();
    }
    
    typename QuadTreeContainer::iterator end()
    {
        return m_AllItems.end();
    }
    
    typename QuadTreeContainer::const_iterator cbegin()
    {
        return m_AllItems.cbegin();
    }
    
    typename QuadTreeContainer::const_iterator cend()
    {
        return m_AllItems.cend();
    }

    void insert(const OBJECT_TYPE& item, const rect& itemsize)
    {
        m_AllItems.push_back(item);
        root.insert(std::prev(m_AllItems.end()), itemsize);
    }

    const std::vector<typename QuadTreeContainer::iterator>& search(const rect& area) const
    {
        listItemPointers.clear();
        listItemPointers.reserve(m_AllItems.size());
        root.search(area, listItemPointers);
        return listItemPointers;
    }

    const void Draw() const
    {
        root.Draw();
    }

};


class SceneStaticQuadTree : public LGE::Scene_t
{

protected:
    struct SomeObjectWithArea
    {
        glm::vec2 vPos;
        glm::vec2 vVel;
        glm::vec2 vSize;
        Color col;
    };

    std::list<SomeObjectWithArea> vecObjects;
    StaticQuadTreeContainer<SomeObjectWithArea> treeObjects;

    float fArea = 100000.0f;

    double msDrawingTime = 0;
    double msDrawingTimeBefore = 0;
    unsigned int drawCalls = 0;

    bool bUseQuadTree = false;

    bool bViewQuadTree = false;
    bool bHeldViewQuadTree = false;

    bool bViewRects = true;
    bool bHeldViewRects = false;

    bool bHeld = false;
    bool bAddRect = false;

public:
    SceneStaticQuadTree()
    {
        LGE::UseTV = true;
        treeObjects.resize({ { 0.0f, 0.0f }, { fArea, fArea } });

        for (int i = 0; i < 100000; ++i)
        {
            SomeObjectWithArea obj;
            obj.vPos = { LGE::rand(0.0f, fArea), LGE::rand(0.0f, fArea) };
            obj.vSize = { LGE::rand(0.1f, 100.0f), LGE::rand(0.1f, 100.0f) };
            obj.col = { LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), 1.0f };
            vecObjects.push_back(obj);
            treeObjects.insert(obj, rect(obj.vPos, obj.vSize));
        }
        SomeObjectWithArea obj;
        obj.vPos = { 0.0f, 0.0f };
        obj.vSize = { 100.0f, 100.0f };
        obj.col = { 1.0f, 0.0f, 0.0f, 1.0f };
        vecObjects.push_back(obj);
        treeObjects.insert(obj, rect(obj.vPos, obj.vSize));

    }

    void OnUpdate(float fElapsedTime) override
    {
        if (LGE::GetMouseButton(1) == GLFW_PRESS && !bAddRect)
        {
            bAddRect = true;
        }

        if (LGE::GetMouseButton(1) == GLFW_RELEASE && bAddRect)
        {
            SomeObjectWithArea obj;
            double as, bs;
            float aw, bw;
            LGE::GetCursorPos(as, bs);
            tv.ScreenToWorld(as, bs, aw, bw);
            obj.vPos = { aw, bw };
            obj.vSize = { 10.0f, 10.0f };
            obj.col = { LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), LGE::rand(0.0f, 1.0f), 1.0f };
            vecObjects.push_back(obj);
            treeObjects.insert(obj, rect(obj.vPos, obj.vSize));

            bAddRect = false;
        }

        if (LGE::GetKey(GLFW_KEY_TAB) == GLFW_PRESS && !bHeld)
            bHeld = true;

        if (LGE::GetKey(GLFW_KEY_TAB) == GLFW_RELEASE && bHeld)
        {
            bHeld = false;
            bUseQuadTree = !bUseQuadTree;
        }
        
        if (LGE::GetKey(GLFW_KEY_X) == GLFW_PRESS && !bHeldViewQuadTree)
            bHeldViewQuadTree = true;

        if (LGE::GetKey(GLFW_KEY_X) == GLFW_RELEASE && bHeldViewQuadTree)
        {
            bHeldViewQuadTree = false;
            bViewQuadTree = !bViewQuadTree;
        }
        
        if (LGE::GetKey(GLFW_KEY_Z) == GLFW_PRESS && !bHeldViewRects)
            bHeldViewRects = true;

        if (LGE::GetKey(GLFW_KEY_Z) == GLFW_RELEASE && bHeldViewRects)
        {
            bHeldViewRects = false;
            bViewRects = !bViewRects;
        }

        float fWorldTLX, fWorldTLY;
        float fWorldBRX, fWorldBRY;

        tv.ScreenToWorld(0, 0, fWorldTLX, fWorldTLY);
        tv.ScreenToWorld(SCREEN_WIDTH, SCREEN_HEIGHT, fWorldBRX, fWorldBRY);

        glm::vec2 vWorldTL = { fWorldTLX, fWorldTLY };
        glm::vec2 vWorldBR = { fWorldBRX, fWorldBRY };

        rect rScreen = { vWorldTL, vWorldBR - vWorldTL };

        if (bViewRects)
        {
            // Using Quad-Trees
            if (bUseQuadTree)
            {
                LGE::Timer time;
                unsigned int calls = 0;
                for (const auto& obj : treeObjects.search(rScreen))
                {
                    calls++;
                    DrawRect(obj->vPos, obj->vSize, obj->col);
                }
                drawCalls = calls;
                msDrawingTime = time.nowMs();
            }

            // Linear
            else
            {
                LGE::Timer time;
                unsigned int calls = 0;
                for (const auto& obj : vecObjects)
                {
                    if (rScreen.overlaps({ obj.vPos, obj.vSize }))
                    {
                        calls++;
                        DrawRect(obj.vPos, obj.vSize, obj.col);
                    }
                }
                drawCalls = calls;
                msDrawingTime = time.nowMs();
            }
        }

        if(bViewQuadTree)
            treeObjects.Draw();

    }


    void OnImGuiRender() override
    {
        ImGui::Text("Took %.3f ms", (msDrawingTime + msDrawingTimeBefore) / 2);
        msDrawingTimeBefore = msDrawingTime;
        ImGui::Text("Draw Calls: %.3u", drawCalls);

        if (bUseQuadTree)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("USING QUAD TREES :) !!!");
            ImGui::PopStyleColor();
        }
        if (!bUseQuadTree)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("NOT USING QUAD TREES ;( ...");
            ImGui::PopStyleColor();
        }
        ImGui::Text("Controls:");
        ImGui::Text("TAB: Switch from Linear to Quad-Tree");
        ImGui::Text("X: Switch for viewing Quad-Tree");
        ImGui::Text("Z: Switch for viewing Rects");
        ImGui::Text("Q: Zoom In");
        ImGui::Text("E: Zoom Out");
        ImGui::Text("Mouse Left-Button: Move");
        ImGui::Text("Mouse Right-Button: Add Obj");


    }

    ~SceneStaticQuadTree()
    {

    }
};

class PixelDrawing : public LGE::Scene_t
{
public:
    PixelDrawing()
    {

    }

    ~PixelDrawing() {}

    void OnUpdate(float fElapsedTime) override
    {
        for (int i = 0; i < SCREEN_WIDTH; ++i)
        {
            for (int j = 0; j < SCREEN_HEIGHT; ++j)
            {
                DrawPixel(i, j, Color{ LGE::rand(0, 1), LGE::rand(0, 1), LGE::rand(0, 1), 1.0 });
            }
        }
    }

    void OnImGuiRender() override
    {
        ImGui::Text("Screen Width = %d", LGE::GetScreenWidth);
        ImGui::Text("Screen Height = %d", LGE::GetScreenHeight);
    }
};

constexpr int ScreenWidth = 1280;
constexpr int ScreenHeight = 960;

constexpr float simMinWidth = 20.0f;
constexpr double simScale = ScreenHeight / simMinWidth;
constexpr float simWidth = ScreenWidth / simScale;
constexpr float simHeight = ScreenHeight / simScale;

class MinutesPhysics : public LGE::Scene_t
{
public:


    float cX(glm::vec2 pos)
    {
        return pos.x * simScale;
    }
    
    float cY(glm::vec2 pos)
    {
        return ScreenHeight - pos.y * simScale;
    }
    
    float pX(glm::vec2 pos)
    {
        return pos.x / simScale;
    }
    
    float pY(glm::vec2 pos)
    {
        return (ScreenHeight - pos.y) / simScale;
    }

    glm::vec2 gravity = { 0.0,-10.0 };
    double timeStep = 1.0 / 60.0;
    float delta = 1.0;
    bool pause = false;
    float restitution = 0.0f;

    struct ball
    {
        float radius = 1.5;
        glm::vec2 pos { 0.2, 0.2 };
        glm::vec2 vel { 10.0, 15.0 };
        Color c = { 1.0f, 0.0f, 0.0f, 1.0f };

        static bool collisionDetection(const ball& b0, const ball& b1)
        {
            auto vDis = b0.pos - b1.pos;
            float d = glm::length(vDis);
            auto radiiSum = (b0.radius + b1.radius);
            return (d != 0.0 && d < radiiSum);
        }

        static void handleCollision(ball& b0, ball& b1, float rest)
        {
            auto dir = b1.pos - b0.pos;
            float d = glm::length(dir);
            auto radiiSum = (b0.radius + b1.radius);

            dir = dir / d;

            float corr = (radiiSum - d) / 2.0f;
            b0.pos.x += dir.x * (-corr);
            b0.pos.y += dir.y * (-corr);
            b1.pos.x += dir.x *  corr;
            b1.pos.y += dir.y *  corr;

            auto v0 = glm::dot(b0.vel, dir);
            auto v1 = glm::dot(b1.vel, dir);

            auto newV0 = (v0 + v1 - (v0 - v1) * rest) / (2.0f);
            auto newV1 = (v0 + v1 - (v1 - v0) * rest) / (2.0f);

            b0.vel.x += dir.x * (newV0 - v0);
            b0.vel.y += dir.y * (newV0 - v0);
            b1.vel.x += dir.x * (newV1 - v1);
            b1.vel.y += dir.y * (newV1 - v1);
        }
    };

    std::vector<ball> balls;

public:
    MinutesPhysics()
    {
        for (int i = 0; i < 10; ++i)
        {
            glm::vec2 loc = { LGE::rand(1.0f, ScreenWidth - 1) , LGE::rand(1.0f, ScreenHeight - 1) };
            balls.emplace_back(20 / (simScale * 2), glm::vec2{ pX(loc), pY(loc) }, glm::vec2{ LGE::rand(-30.0f, 30.0f), LGE::rand(-30.0f, 30.0f) });
        }
    }

    ~MinutesPhysics() { std::cout << "Calling destructor"; }

    void handleWallColl(ball& b0, float simWidth, float simHeight)
    {
        if (b0.pos.x < b0.radius)
        {
            b0.pos.x = b0.radius;
            b0.vel.x = -b0.vel.x;
        }
        if (b0.pos.x > simWidth)
        {
            b0.pos.x = simWidth;
            b0.vel.x = -b0.vel.x;
        }
        if (b0.pos.y < b0.radius)
        {
            b0.pos.y = b0.radius;
            b0.vel.y = -b0.vel.y;
        }
        if (b0.pos.y > simHeight)
        {
            b0.pos.y = simHeight;
            b0.vel.y = -b0.vel.y;
        }
    }

    void simulate(float fElapsedTime)
    {
        timeStep = fElapsedTime * delta;

        for (ball& b0 : balls)
        {
            b0.vel.x += gravity.x * timeStep;
            b0.vel.y += gravity.y * timeStep;

            b0.pos.x += b0.vel.x * timeStep;
            b0.pos.y += b0.vel.y * timeStep;

            bool coll = false;
            for (ball& b1 : balls)
            {
                if (ball::collisionDetection(b0, b1))
                {
                    ball::handleCollision(b0, b1, restitution);
                    coll = true;
                    b0.c.y = 1.0f;
                }
            }
            if (!coll)
                b0.c.y = 0.0f;

            handleWallColl(b0, simWidth, simHeight);

        }
    }

    void draw()
    {
        for (ball& b : balls)
        {
            DrawPoint(cX(b.pos), cY(b.pos), b.radius * simScale * 2, b.c);
        }
    }


    bool addBallHeld = false;
    void OnUpdate(float fElapsedTime) override
    {
        if(!pause)
            simulate(fElapsedTime);

        draw();
        double mouseX, mouseY;
        LGE::GetCursorPos(mouseX, mouseY);
        glm::vec2 mousePos(mouseX, mouseY);
        if (LGE::GetMouseButton(0) == GLFW_PRESS)
            addBallHeld = true;

        if (LGE::GetMouseButton(0) == GLFW_RELEASE && addBallHeld == true)
        {
            balls.emplace_back(20/ (simScale*2), glm::vec2{ pX(mousePos), pY(mousePos)}, glm::vec2{LGE::rand(-30.0f, 30.0f), LGE::rand(-30.0f, 30.0f)});
            addBallHeld = false;
        }

    }

    void OnImGuiRender() override
    {
        ImGui::Text("Has %d balls", balls.size());
        if (ImGui::Button("Pause"))
            pause = !pause;
        ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("TimeStep", &delta, 0.1f, 0.1f, 10.0f);
    }
};


class TicTacToe : public LGE::Scene_t
{
public:
    TicTacToe()
    {

    }

    ~TicTacToe() {}

    void OnUpdate(float fElapsedTime) override
    {
        DrawPoint(200, 200, 20, { 1.0, 1.0, 0.0, 1.0 });
    }

    void OnImGuiRender() override
    {
        ImGui::Text("Hello Imgui!");
    }
};


int main(int argc, char** argv)
{
    LGE::Application Demo;
    Demo.RegisterScene<SceneStaticQuadTree>("Static Quad Tree");
    Demo.RegisterScene<PixelDrawing>("Pixel Drawing");
    Demo.RegisterScene<MinutesPhysics>("10 Minutes Physics");
    Demo.RegisterScene<TicTacToe>("TicTacToe");
    Demo.Run ();

    return 0;
}
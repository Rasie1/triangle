#pragma once

#include <array>
#include <utility>
#include <cmath>

namespace Triangle {

namespace DefaultConstants
{
    const float Side = 173.20508075689f;
    const float Height = 150.f;
    const float HeightK = 0.86692549378f;
    const float Center = 0.5773502f;
}

struct Pos 
{
    int x, y, s;
};

inline bool operator==(const Pos& lhs, const Pos& rhs)
{
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.s == rhs.s;
}

inline bool operator!=(const Pos& lhs, const Pos& rhs)
{
    return lhs.x != rhs.x
        && lhs.y != rhs.y
        && lhs.s != rhs.s;
}

using Vec2 = std::pair<float, float>;

inline Vec2 gridToWorld(
    const Pos& pos, 
    float worldspaceTriangleSide, 
    float worldspaceTriangleHeight) 
{
    return Vec2{
        pos.x * worldspaceTriangleSide + pos.y * worldspaceTriangleSide / 2.f,
        pos.y * worldspaceTriangleHeight
    };
}

inline Vec2 gridToWorld(const Pos& pos) 
{
    return Vec2{
        pos.x * DefaultConstants::Side + pos.y * DefaultConstants::Side / 2.f,
        pos.y * DefaultConstants::Height
    };
}

inline Pos worldToGrid(
    const Vec2& pos, 
    float worldspaceTriangleSide, 
    float worldspaceTriangleHeight) 
{
    float y = std::get<1>(pos) / worldspaceTriangleHeight;
    float x = std::get<0>(pos) / worldspaceTriangleSide - y / 2.f;
    bool s = (x - floor(x) + y - floor(y)) > 1.f;
    return Pos{int(x), int(y), int(s)};
}

inline Pos worldToGrid(const Vec2& pos) 
{
    float y = std::get<1>(pos) / DefaultConstants::Height;
    float x = std::get<0>(pos) / DefaultConstants::Side - y / 2.f;
    bool s = (x - floor(x) + y - floor(y)) > 1.f;
    return Pos{int(x), int(y), int(s)};
}


inline int toIndex(const Pos& pos, int w)
{
    return pos.x * 2 + pos.y * w * 2 + pos.s;
}

inline int toIndex(int x, int y, int s, int w)
{
    return x * 2 + y * w * 2 + s;
}

inline Pos fromIndex(int i, int w)
{
    return Pos{(i / 2) % w, (i / 2) / w, i % 2};
}

inline int anotherSide(int s)
{
    return (s == 0) ? 1 : 0;
}

inline std::array<Pos, 3> adjacentCoords(const Pos& pos) 
{
    int x = pos.x;
    int y = pos.y;
    int s = pos.s;
    int x1 = x;
    int y1 = y;
    int s1 = anotherSide(s);
    int x2 = s ? (x + 1) : x;
    int y2 = s ? y : (y - 1);
    int s2 = anotherSide(s);
    int x3 = s ? x : (x - 1);
    int y3 = s ? (y + 1) : y;
    int s3 = anotherSide(s);
    return std::array<Pos, 3>{
        Pos{x1, y1, s1}, 
        Pos{x2, y2, s2},
        Pos{x3, y3, s3}
    };
}

inline std::pair<int, int> gridToIntersectionPos(int i, int w) 
{
    return std::make_pair(i % (w + 1), i / (w + 1));
}

inline int intersectionPosToIndex(int x, int y, int w)
{
    return x + y * (w + 1);
}

inline int distance(const Pos& a, const Pos& b)
{
    auto dx = a.x - b.x;
    auto dy = a.y - b.y;
    auto ds = a.s - b.s;
    return abs(dx) + abs(dy) + abs(dx + dy + ds);
}

}
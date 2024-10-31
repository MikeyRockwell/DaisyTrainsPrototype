#pragma once

#include "Engine.h"
#include <functional>

struct Vector2Int 
{
    int x, y;

    // Define the '<' operator for Vector2Int.
    bool operator<(const Vector2Int& other) const {
        // First compare by 'x', then by 'y'.
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};

// Comparator function
struct Vector2IntComparator 
{
    bool operator()(const Vector2Int& a, const Vector2Int& b) const {
        if (a.x < b.x) return true;
        if (a.x > b.x) return false;
        return a.y < b.y;
    }
};

// Hash function
struct Vector2IntHash 
{
    size_t operator()(const Vector2Int& v) const {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }
};

// Equality function
struct Vector2IntEqual 
{
    bool operator()(const Vector2Int& lhs, const Vector2Int& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

inline Vector2 LocalToWorldPosition(Vector2 parentWorldPosition, float parentAngle, Vector2 localPosition)
{
    Vector2 result = Vector2Rotate(localPosition, parentAngle * DEG2RAD);
    return Vector2Add(result, parentWorldPosition);
}
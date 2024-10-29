#pragma once

#include "raylib.h"

Vector2 BezierQuadratic(Vector2 p0, Vector2 p1, Vector2 p2, float t)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;

    Vector2 point{};
    point.x = uu * p0.x + 2 * u * t * p1.x + tt * p2.x;
    point.y = uu * p0.y + 2 * u * t * p1.y + tt * p2.y;

    return point;
}

Vector2 BezierCubic(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Vector2 point{};
    point.x = uuu * p0.x; // (1-t)^3 * p0
    point.x += 3 * uu * t * p1.x; // 3 * (1-t)^2 * t * p1
    point.x += 3 * u * tt * p2.x; // 3 * (1-t) * t^2 * p2
    point.x += ttt * p3.x; // t^3 * p3

    point.y = uuu * p0.y;
    point.y += 3 * uu * t * p1.y;
    point.y += 3 * u * tt * p2.y;
    point.y += ttt * p3.y;

    return point;
}

Vector2 MidPoint(Vector2 a, Vector2 b)
{
    return Vector2{ (a.x + b.x) / 2, (a.y + b.y) / 2 };
}
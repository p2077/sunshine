#pragma once
#include "raylib.h"
#include "Math.h"
#include <array>
#include <vector>
#include <algorithm>

struct Circle
{
    Vector2 position;
    float radius;
};

struct Capsule
{
    Vector2 position;
    Vector2 direction;
    float radius;
    float halfHeight;
};

bool CheckCollisionCircles(Circle circle1, Circle circle2)
{
    Vector2 delta = circle2.position - circle1.position;
    float radiiSum = circle1.radius + circle2.radius;
    return Length(delta) <= radiiSum;
}

// Mtv resolves circle2 from circle1
bool CheckCollisionCircles(Circle circle1, Circle circle2,
    Vector2& mtv)
{
    Vector2 delta = circle2.position - circle1.position;
    float radiiSum = circle1.radius + circle2.radius;
    float centerDistance = Length(delta);
    bool collision = centerDistance <= radiiSum;
    if (collision)
    {
        // Compute mtv (sum of radii - distance between centers)
        float mtvDistance = radiiSum - centerDistance;
        mtv = Normalize(delta) * mtvDistance;
    }
    return collision;
}

// Projects point P along line AB
Vector2 Project(Vector2 a, Vector2 b, Vector2 p)
{
    Vector2 AB = b - a;
    float t = Dot((p - a), AB) / LengthSqr(AB);
    return a + AB * std::min(std::max(t, 0.0f), 1.0f);
}

void NearestCirclePoints(Capsule capsule1, Capsule capsule2, Vector2& nearest1, Vector2& nearest2)
{
    Vector2 top1 = capsule1.position + capsule1.direction * capsule1.halfHeight;
    Vector2 top2 = capsule2.position + capsule2.direction * capsule2.halfHeight;
    Vector2 bot1 = capsule1.position - capsule1.direction * capsule1.halfHeight;
    Vector2 bot2 = capsule2.position - capsule2.direction * capsule2.halfHeight;

    std::array<Vector2, 4> lines
    {
        top2 - top1,
        bot2 - top1,
        top2 - bot1,
        bot2 - bot1,
    };

    size_t min = 0;
    for (size_t i = 1; i < lines.size(); i++)
    {
        if (LengthSqr(lines[i]) < LengthSqr(lines[min]))
            min = i;
    }

    nearest1 = min < 2 ? top1 : bot1;
    nearest2 = Project(bot2, top2, nearest1);
    nearest1 = Project(bot1, top1, nearest2);
}

bool CheckCollisionCapsules(Capsule capsule1, Capsule capsule2, Vector2& mtv)
{
    Vector2 nearest1, nearest2;
    NearestCirclePoints(capsule1, capsule2, nearest1, nearest2);
    return CheckCollisionCircles({ nearest1, capsule1.radius }, { nearest2, capsule2.radius }, mtv);
}

bool CheckCollisionLineCircle(Vector2 lineStart, Vector2 lineEnd, Circle circle)
{
    Vector2 nearest = NearestPoint(lineStart, lineEnd, circle.position);
    return DistanceSqr(nearest, circle.position) <= circle.radius * circle.radius;
}

bool CheckCollisionLineRec(Vector2 lineStart, Vector2 lineEnd, Rectangle rectangle)
{
    float xMin = rectangle.x;
    float xMax = rectangle.x + rectangle.width;
    float yMin = rectangle.y;
    float yMax = rectangle.y + rectangle.height;

    std::array<Vector2, 4> points
    {
        Vector2 {xMin, yMin},   // top left
        Vector2 {xMax, yMin},   // top right
        Vector2 {xMax, yMax},   // bot right
        Vector2 {xMin, yMax},   // bot left
    };

    for (size_t i = 0; i < points.size(); i++)
    {
        if (CheckCollisionLines(lineStart, lineEnd, points[i], points[(i + 1) % points.size()], nullptr))
            return true;
    }
    return false;
}

bool CheckCollisionLineRec(Vector2 lineStart, Vector2 lineEnd, Rectangle rectangle, Vector2& poi)
{
    float xMin = rectangle.x;
    float xMax = rectangle.x + rectangle.width;
    float yMin = rectangle.y;
    float yMax = rectangle.y + rectangle.height;

    std::array<Vector2, 4> points
    {
        Vector2 {xMin, yMin},   // top left
        Vector2 {xMax, yMin},   // top right
        Vector2 {xMax, yMax},   // bot right
        Vector2 {xMin, yMax},   // bot left
    };

    std::array<Vector2, 4> intersections;
    intersections.assign(Vector2One() * 100000.0f);

    bool collision = false;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (CheckCollisionLines(lineStart, lineEnd, points[i], points[(i + 1) % points.size()], &intersections[i]))
        {
            collision = true;
        }
    }

    if (collision)
    {
        poi = *std::min_element(intersections.begin(), intersections.end(),
            [&lineStart](const Vector2& a, const Vector2& b)
            {
                return DistanceSqr(lineStart, a) < DistanceSqr(lineStart, b);
            }
        );
    }

    return collision;
}

// Determines if circle is visible from line start
bool IsCircleVisible(Vector2 lineStart, Vector2 lineEnd, Circle circle, const std::vector<Rectangle>& obstacles)
{
    if (!CheckCollisionLineCircle(lineStart, lineEnd, circle)) return false;
    float targetDistance = DistanceSqr(lineStart, circle.position);

    std::vector<Vector2> intersections(obstacles.size());
    for (const Rectangle& obstacle : obstacles)
    {
        Vector2 poi;
        bool intersects = CheckCollisionLineRec(lineStart, lineEnd, obstacle, poi);
        if (intersects)
        {
            if (DistanceSqr(lineStart, poi) < targetDistance) return false;
        }
    }

    return true;
}

// Determines if rectangle is visible from line start
bool IsRectangleVisible(Vector2 lineStart, Vector2 lineEnd, Rectangle rectangle, const std::vector<Rectangle>& obstacles)
{
    if (!CheckCollisionLineRec(lineStart, lineEnd, rectangle)) return false;
    float targetDistance = DistanceSqr(lineStart, 
        { rectangle.x + rectangle.width * 0.5f, rectangle.y + rectangle.height * 0.5f});

    std::vector<Vector2> intersections(obstacles.size());
    for (const Rectangle& obstacle : obstacles)
    {
        Vector2 poi;
        bool intersects = CheckCollisionLineRec(lineStart, lineEnd, obstacle, poi);
        if (intersects)
        {
            if (DistanceSqr(lineStart, poi) < targetDistance) return false;
        }
    }

    return true;
}

bool NearestIntersection(Vector2 lineStart, Vector2 lineEnd, const std::vector<Rectangle>& obstacles, Vector2& poi)
{
    bool collision = false;
    std::vector<Vector2> intersections(obstacles.size());
    for (const Rectangle& obstacle : obstacles)
    {
        if (CheckCollisionLineRec(lineStart, lineEnd, obstacle, poi))
        {
            intersections.push_back(poi);
            collision = true;
        }
    }

    if (collision)
    {
        poi = *std::min_element(intersections.begin(), intersections.end(),
            [&lineStart](const Vector2& a, const Vector2& b)
            {
                return DistanceSqr(lineStart, a) < DistanceSqr(lineStart, b);
            }
        );
    }

    return collision;
}

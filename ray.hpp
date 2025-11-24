#include <vector.hpp>

#pragma once

class Ray {
public:
    Point origin;
    Vector direction;
    Ray(Point, Vector);
    // Computes origin + t * direction
    Point at(float t) const;
};
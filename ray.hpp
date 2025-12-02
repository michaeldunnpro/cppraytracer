#pragma once

#include "vector.hpp"

class Ray {
public:
    Point origin;
    Vector direction;
    Ray(Point, Vector);
    // Computes origin + t * direction
    Point at(float t) const;
};
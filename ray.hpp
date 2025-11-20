#include <vector.hpp>

#pragma once

class Ray {
public:
    Point origin;
    Vector direction;
    // Computes origin + t * direction
    Point at(double t) const;
};
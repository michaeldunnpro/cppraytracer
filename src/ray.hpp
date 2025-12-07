#pragma once

#include "vector.hpp"

/*
 * INCLUDE DESCRPTION IN HEADER FILE 
 */
class Ray {
public:
    Point origin;
    Vector direction;
    Ray(Point, Vector);
    // Computes origin + t * direction
    Point at(float t) const;
};

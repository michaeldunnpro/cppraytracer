#pragma once

#include "vector.hpp"

/**
 * @brief Data of a ray, consisting of an point `origin` and a vector
 * `direction` (not normalized)
 */
class Ray {
public:
    Point origin;
    Vector direction;

    Ray(Point, Vector);

    /**
     * @brief Compute `origin + t * direction`.
     */
    Point at(float t) const;
};

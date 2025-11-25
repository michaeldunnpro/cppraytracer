#include <vector>

#include "ray.hpp"
#include "vector.hpp"

#pragma once

class Scene {
public:
    float get_ambient() const;
    float get_specular() const;
    float get_sp() const;

    std::vector<Point> const& get_point_lights() const;
    /**
     * @param recursion_depth maximum recursion depth allowed; 0 for no recursion
     */
    Color trace(Ray const& ray, int recursion_depth) const;
};
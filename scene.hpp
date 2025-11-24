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
    Color trace(Ray const&) const;
};
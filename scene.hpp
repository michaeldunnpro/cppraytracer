#include <vector>

#include "ray.hpp"
#include "vector.hpp"

#pragma once

class Scene {
public:
    double get_ambient() const;
    double get_specular() const;
    double get_sp() const;

    std::vector<Point> const& get_point_lights() const;
    Color trace(Ray const&) const;
};
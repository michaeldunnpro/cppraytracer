#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

// Forward declaration
class Shape;

class Scene {
private:
    std::vector<Shape> shapes;
    std::vector<Point> point_lights;

    /**
     * @brief Compute the first point a ray intersects among all shapes
     * @param ray The ray
     * @return A pair `(t, shape)` where `t` is the parameter indicating
     * the intersection position as in other methods, and `shape` is a
     * const lvalue reference to the shape being intersected.
     */
    std::optional<std::pair<float, Shape const*>> intersect_first_all(Ray const& ray) const;

public:
    float get_ambient() const;
    float get_specular() const;
    float get_sp() const;

    /**
     * @brief Get the location of all point light sources visible from `point`.
     */
    std::vector<Point> get_visible_point_lights(Point const& point) const;

    /**
     * @param recursion_depth maximum recursion depth allowed; 0 for no recursion
     */
    Color trace(Ray const& ray, int recursion_depth) const;
};
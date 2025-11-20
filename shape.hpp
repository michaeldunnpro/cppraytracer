#include <memory>
#include <optional>

#include "color.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

#pragma once

/**
 * Represents a shape that can be rendered.
 * All shapes are assumed to be double-sided.
 */
class Shape {
public:
    virtual ~Shape() { };

    /**
     * @return The smallest positive parameter `t` among the intersections
     * if it exists, and empty otherwise. When it exists, the
     * intersection point is given by `ray.at(t)`.
     * @note Using `t` instead of the intersection point makes it easier
     * to compare which intersection points are first hit among all shapes.
     */
    virtual std::optional<double> intersect_first(Ray const&) const = 0;

    /**
     * @return A unit normal vector at the given point on the surface.
     * The direction is not guaranteed.
     * @note By separating `normal_at()` and `material_at()`, there could be
     * potential redundant calculation (e.g., calculate surface parameter
     * from point). However, addressing that would be an overoptimization
     * and would also reduce code readability.
     */
    virtual Vector normal_at(Point const&) = 0;

    // fn (&self, &Point) -> Box<dyn Material>
    /**
     * @return The material at the given point on the surface.
     */
    virtual std::unique_ptr<Material> material_at(Point const&) const = 0;
};

// We use `BasicSphere<T: Material + Clone> { material: T }` instead of
// `BasicSphere { material: Box<dyn (Material + Clone)> }`
/**
 * A sphere with a single material.
 */
template <typename T = BasicMaterial>
class BasicSphere : Shape {
private:
    Point center;
    double radius;
    T material;

public:
    BasicSphere(Point center, double radius, T material);
    ~BasicSphere();
    std::optional<double> intersect_first(Ray const&) const override;
    Vector normal_at(Point const&) const override;
    std::unique_ptr<Material> material_at(Point const&) const override;
};

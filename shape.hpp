#pragma once

#include <cmath>
#include <memory>
#include <optional>

#include "color.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

// Forward declaration
class Material;
class BasicMaterial;

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
    virtual std::optional<float> intersect_first(Ray const&) const = 0;

    /**
     * @return A unit normal vector at the given point on the surface.
     * The direction is not guaranteed.
     * @note By separating `normal_at()` and `material_at()`, there could be
     * potential redundant calculation (e.g., calculate surface parameter
     * from point). However, addressing that would be an overoptimization
     * and would also reduce code readability.
     */
    virtual Vector normal_at(Point const&) const = 0;

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
class BasicSphere : public Shape {
private:
    Point center;
    float radius;
    T material;

public:
    BasicSphere(Point center, float radius, T material);
    std::optional<float> intersect_first(Ray const&) const override;
    Vector normal_at(Point const&) const override;
    std::unique_ptr<Material> material_at(Point const&) const override;
};

/**
 * A plane with a single material.
 */
template <typename T = BasicMaterial>
class BasicPlane : public Shape {
private:
    Point point; // Any point on the plane
    Vector normal; // A unit normal vector
    T material;

public:
    BasicPlane(Point point, Vector normal, T material);
    std::optional<float> intersect_first(Ray const&) const override;
    Vector normal_at(Point const&) const override;
    std::unique_ptr<Material> material_at(Point const&) const override;
};

// Template definition; must be put or otherwise included in the header

template <typename T>
inline BasicSphere<T>::BasicSphere(Point center, float radius, T material)
    : center(center)
    , radius(radius)
    , material(material) {
}

template <typename T>
std::optional<float> BasicSphere<T>::intersect_first(Ray const& ray) const {
    float a = ray.direction * ray.direction;
    float b = 2 * ray.direction * (ray.origin - this->center);
    float c = (ray.origin - this->center) * (ray.origin - this->center) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return {};
    }
    float root = std::sqrt(discriminant);
    float t1 = (-b - root) / (2 * a);
    float t2 = (-b + root) / (2 * a);
    if (t1 > 0) {
        return t1;
    }
    if (t2 > 0) {
        return t2;
    }
    return {};
}

template <typename T>
Vector BasicSphere<T>::normal_at(Point const& point) const {
    return !(point - this->center);
}

template <typename T>
std::unique_ptr<Material> BasicSphere<T>::material_at(Point const&) const {
    return std::make_unique<T>(this->material);
}

template <typename T>
inline BasicPlane<T>::BasicPlane(Point point, Vector normal, T material)
    : point(point)
    , normal(!normal)
    , material(material) {
}

template <typename T>
std::optional<float> BasicPlane<T>::intersect_first(Ray const& ray) const {
    float div = ray.direction * this->normal;
    if (std::abs(div) < 1e-6) {
        return {};
    }
    float t = ((ray.origin - this->point) * this->normal) / div;
    return t > 0 ? std::optional<float>(t) : std::nullopt;
}

template <typename T>
Vector BasicPlane<T>::normal_at(Point const&) const {
    return normal;
}

template <typename T>
std::unique_ptr<Material> BasicPlane<T>::material_at(Point const&) const {
    return std::unique_ptr<Material>(this->material);
}

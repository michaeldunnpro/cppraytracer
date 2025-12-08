#pragma once

#include "../shape.hpp"

/**
 * @brief A generic plane with `material_at()` unimplemented
 */
class Plane : public Shape {
private:
    Point point; // Any point on the plane
    Vector normal; // A unit normal vector

public:
    Plane(Point point, Vector normal);
    std::optional<float> intersect_first(Ray const&) const override;
    Vector normal_at(Point const&) const override;
};

/**
 * A plane with a single material.
 */
template <typename T = BasicMaterial>
class BasicPlane : public Plane {
private:
    T material;

public:
    BasicPlane(Point point, Vector normal, T material);
    std::unique_ptr<Material> material_at(Point const&) const override;
};

// Template definition; must be put or otherwise included in the header

template <typename T>
inline BasicPlane<T>::BasicPlane(Point point, Vector normal, T material)
    : Plane(point, normal)
    , material(material) {
}

template <typename T>
std::unique_ptr<Material> BasicPlane<T>::material_at(Point const&) const {
    return std::make_unique<T>(this->material);
}

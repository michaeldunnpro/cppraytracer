#pragma once

#include "../shape.hpp"

/**
 * @brief A generic sphere with `material_at()` unimplemented.
 */
class Sphere : public Shape {
protected:
    Point center;
    float radius;

public:
    Sphere(Point center, float radius);
    std::optional<float> intersect_first(Ray const&) const override;
    Vector normal_at(Point const&) const override;
};

// We use `BasicSphere<T: Material + Clone> { material: T }` instead of
// `BasicSphere { material: Box<dyn (Material + Clone)> }`
/**
 * A sphere with a single material.
 */
template <typename T = BasicMaterial>
class BasicSphere : public Sphere {
private:
    T material;

public:
    BasicSphere(Point center, float radius, T material);
    std::unique_ptr<Material> material_at(Point const&) const override;
};

// Template definition; must be put or otherwise included in the header

template <typename T>
inline BasicSphere<T>::BasicSphere(Point center, float radius, T material)
    : Sphere(center, radius)
    , material(material) {
}

template <typename T>
std::unique_ptr<Material> BasicSphere<T>::material_at(Point const&) const {
    return std::make_unique<T>(this->material);
}

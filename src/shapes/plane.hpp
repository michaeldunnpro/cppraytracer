#pragma once

#include "../shape.hpp"

/**
 * @brief A generic plane with `material_at()` unimplemented
 */
class Plane : public Shape {
protected:
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

/**
 * @brief A parametric plane on which material patterns can be added.
 * @tparam Func A function type `(float, float) -> std::unique_ptr<Material>`
 */
template <typename Func>
class ParametricPlane : public Plane {
private:
    Vector v;
    Vector w;
    // The material at `point + a * v + b * w` is given by material_fn(a, b)
    Func material_fn;

public:
    ParametricPlane(Point point, Vector v, Vector w, Func material_fn);
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

template <typename Func>
inline ParametricPlane<Func>::ParametricPlane(Point point, Vector v, Vector w, Func material_fn)
    : Plane(point, v ^ w)
    , v(v)
    , w(w)
    , material_fn(material_fn) {
}

template <typename Func>
inline std::unique_ptr<Material> ParametricPlane<Func>::material_at(Point const& point) const {
    // Compute the parameters
    // solve for a v + b w + c n = (point - this->point), where n is the normal
    // a and b will be the parameters; c should theoretically be zero
    Vector param = lin_solve(this->v, this->w, this->normal, point - this->point);
    return this->material_fn(param.x, param.y);
}

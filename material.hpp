#include "color.hpp"
#include "scene.hpp"
#include "vector.hpp"

#pragma once

/**
 * Represent the shading behavior at a single point.
 */
class Material {
public:
    virtual ~Material() = default;
    /**
     * @param normal the unit vector pointing out of the surface
     */
    virtual Color get_color(
        Vector const& incoming, Point const& point,
        Vector const& normal, Scene const* scene) const
        = 0;
};

/**
 * An implementation of `Material` based on the minimal
 * project requirement
 */
class BasicMaterial : Material {
private:
    Color const color;
    double const refl;

public:
    BasicMaterial(Color, double);
    Color get_color(
        Vector const& incoming, Point const& point,
        Vector const& normal, Scene const* scene) const override;
};

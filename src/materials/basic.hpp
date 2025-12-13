#pragma once

#include "../material.hpp"

/**
 * An implementation of `Material` based on the minimal
 * project requirement
 */
class BasicMaterial : public Material {
private:
    Color const color;
    float const refl; // reflectivity

public:
    BasicMaterial(Color, float);
    Color get_color(
        Vector const& incoming, Point const& point, Vector const& normal,
        Scene const* scene, int recursion_depth) const override;
};

#pragma once

#include "../material.hpp"

/**
 * @brief Completely transparent material that can only reflect or refract.
 * Currently doesn't work if two transparent objects have intersection.
 */
class TransparentMaterial : public Material {
private:
    float ior; // Index of refraction of inside / index of refraction of outside

public:
    TransparentMaterial(float ior);
    Color get_color(
        Vector const& incoming, Point const& point, Vector const& normal,
        Scene const* scene, int recursion_depth) const override;
};

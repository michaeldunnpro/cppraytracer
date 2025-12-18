#pragma once

#include <cstdint>

#include "../material.hpp"

/**
 * @brief Cook-Torrance model with importance sampling for specular reflection.
 */
class PBRMaterial : public Material {
private:
    Color color;
    float roughness; // [0, 1]
    float metallic; // [0, 1], usually binary
    // Reflectance used for non-metallic surfaces; remapped according to
    // https://google.github.io/filament/Filament.md.html#materialsystem/parameterization/remapping/reflectanceremapping
    // Usually [0, 1], but can be higher
    float reflectance;
    int num_samples;

public:
    PBRMaterial(Color color, float roughness, float metallic, float reflectance = 0.5f, int num_samples = 64);
    Color get_color(
        Vector const& incoming, Point const& point, Vector const& normal,
        Scene const* scene, int recursion_depth) const override;
};

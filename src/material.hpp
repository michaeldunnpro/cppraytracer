#pragma once

#include "color.hpp"
#include "scene.hpp"
#include "vector.hpp"

// Forward declaration
class Scene;

/**
 * Represent the shading behavior at a single point.
 */
class Material {
public:
    virtual ~Material() = default;
    /**
     * @brief Computes the color when an instance of the material is seen.
     * @param incoming the direction of the incoming ray from which
     * the material is seen
     * @param point the location of the material
     * @param normal the orientation of the material, i.e.,
     * the unit vector pointing out of the object (not necessarily
     * towards the incoming ray)
     * @param scene the scene (required for light sources and tracing reflections)
     * @param recursion_depth maximum recursion depth allowed; 0 for no recursion
     */
    virtual Color get_color(
        Vector const& incoming, Point const& point, Vector const& normal,
        Scene const* scene, int recursion_depth) const = 0;
};

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

/**
 * @brief Cook-Torrance model without sampling.
 */
class PBRMaterial : public Material {
private:
    Color color;
    float roughness; // [0, 1]
    // [0, 1], usually binary; but without sampling metals look really bad
    float metallic;
    // Reflectance used for non-metallic surfaces; remapped according to
    // https://google.github.io/filament/Filament.md.html#materialsystem/parameterization/remapping/reflectanceremapping
    float reflectance;
    int num_samples;
public:
    PBRMaterial(Color color, float roughness, float metallic, float reflectance = 0.5f, int num_samples = 64);
    Color get_color(
        Vector const& incoming, Point const& point, Vector const& normal,
        Scene const* scene, int recursion_depth) const override;
};

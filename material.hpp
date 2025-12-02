#pragma once

#include "color.hpp"
#include "scene.hpp"
#include "vector.hpp"

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
     * the unit vector pointing out of the surface
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
class BasicMaterial : Material {
private:
    Color const color;
    float const refl; // reflexivity

public:
    BasicMaterial(Color, float);
    Color get_color(
        Vector const& incoming, Point const& point, Vector const& normal,
        Scene const* scene, int depth) const override;
};

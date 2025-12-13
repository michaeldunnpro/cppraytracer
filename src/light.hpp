#pragma once

#include "color.hpp"
#include "scene.hpp"
#include "vector.hpp"

// Forward declaration
class Scene;

class Light {
public:
    virtual ~Light() = default;
    /**
     * @brief Get the intensity/color (they are the same thing)
     * of the light at a given point.
     */
    virtual Color get_intensity(Point point) const = 0;
    /**
     * @brief Get the direction pointing from the given point to the
     * light source (not necessarily unit vector)
     */
    virtual Vector get_direction(Point point) const = 0;
    /**
     * @brief Check visibility of the light source from a point in a scene.
     * @param point The point.
     * @param scene The scene.
     * @return Whether the light is visible from the point.
     */
    virtual bool is_visible(Point point, Scene const& scene) const = 0;
};

/**
 * @brief Intermediate implementation of a light located at a point, but
 * with unknown intensity.
 */
class PointLight : public Light {
protected:
    Point position;

public:
    PointLight(Point position);
    Vector get_direction(Point point) const override;
    bool is_visible(Point point, Scene const& scene) const override;
};

/**
 * @brief Basic point light that aligns with the project requirement,
 * i.e., that the intensity is the same everywhere, and in addition
 * allows non-white colors.
 */
class BasicPointLight : public PointLight {
private:
    Color color;

public:
    BasicPointLight(Point position, Color color = Color::white());
    Color get_intensity(Point point) const override;
};

/**
 * @brief Point light whose intensity is inversely proportional to
 * distance to light source squared.
 */
class InverseSquarePointLight : public PointLight {
private:
    Color color; // color/intensity at distance 1

public:
    // intensity is a redundant parameter; it just scales the color
    InverseSquarePointLight(Point position, Color color = Color::white(), float intensity = 1.0f);
    Color get_intensity(Point point) const override;
};

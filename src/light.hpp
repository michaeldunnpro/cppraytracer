#pragma once

#include "color.hpp"
#include "scene.hpp"
#include "vector.hpp"

// Forward declaration
class Scene;

class Light {
public:
    virtual ~Light() = default;
    virtual Color get_intensity(Point point) const = 0;
    virtual Vector get_direction(Point point) const = 0;
    virtual bool is_visible(Point point, Scene const& scene) const = 0;
};

class PointLight : public Light {
protected:
    Point position;

public:
    PointLight(Point position);
    Vector get_direction(Point point) const override;
    bool is_visible(Point point, Scene const& scene) const override;
};

class BasicPointLight : public PointLight {
private:
    Color color;

public:
    BasicPointLight(Point position, Color color = Color::white());
    Color get_intensity(Point point) const override;
};

class InverseSquarePointLight : public PointLight {
private:
    Color color; // not the same unit

public:
    // intensity is redundant; it just scales color
    InverseSquarePointLight(Point position, Color color = Color::white(), float intensity = 1.0f);
    Color get_intensity(Point point) const override;
};

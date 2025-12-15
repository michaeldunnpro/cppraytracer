// Simple enough that we can put it all in a header
#pragma once

#include "color.hpp"
#include "material.hpp"
#include "materials/basic.hpp"
#include "materials/pbr.hpp"
#include "materials/transparent.hpp"
#include "shape.hpp"
#include "shapes/plane.hpp"
#include "shapes/sphere.hpp"
#include "ui.hpp"
#include "vector.hpp"

// Convenience function to create a color (sRGB)
// If want raw/linear RGB then set gamma = 1.0f
// Range [0, 255]
Color rgb(int r, int g, int b, float gamma = 2.2f) {
    return Color::from_rgb(r, g, b, gamma);
}

// Convenience function to create a camera at point a with orientation b
// By default camera is at origin looking towards x-axis, which is now
// overridden.
Camera cam(Point a = Point(0.5f, -1.5f, 0.5f), Vector b = Vector(0, 1, 0)) {
    return Camera(a, b);
}

// Convenience function to create a basic material with specified color and reflectivity
BasicMaterial mat(Color color, float reflectivity = 0.5f) {
    return BasicMaterial(color, reflectivity);
}

// Convenience function to create a scene with specified camera, screen, lighting, and background color
Scene scene(Camera& camera, Screen& screen, float ambient = 0.8f, float specular = 0.5f, float sp = 8.0f, Color background = rgb(135, 206, 235)) {
    return Scene(&camera, &screen, ambient, specular, sp, background);
}

// Convenience function to create a screen with specified width and height
Screen screen(float width = 10.0f, float height = 10.0f) {
    return Screen(width, height);
}

// Convenience function to create a basic sphere shape
template <typename T>
void sphere(Point center, float radius, T material, Scene& scene) {
    scene.add_shape<BasicSphere<T>>(center, radius, material);
}

// Convenience function to create a basic plane shape
template <typename T>
void plane(Point point, Vector normal, T material, Scene& scene) {
    scene.add_shape<BasicPlane<T>>(point, normal, material);
}

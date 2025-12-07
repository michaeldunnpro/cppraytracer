// Simple enough that we can put it all in a header

#include "color.hpp"
#include "material.hpp"
#include "shape.hpp"
#include "ui.hpp"
#include "vector.hpp"

Camera cam(Point a = Point(0, 0, 0), Vector b = Vector(0, 0, 1)) {
    // Convenience function to create a camera at point a with orientation b
    // By default camera is at origin looking down z-axis
    return Camera(a, b);
}

BasicMaterial mat(Color color, float reflectivity) {
    // Convenience function to create a basic material with specified color and reflectivity
    return BasicMaterial(color, reflectivity);
}

Scene scene(Camera& camera, Screen& screen, float ambient = 0.8f, float specular = 0.5f, float sp = 8.0f, Color background = Color(135, 206, 235)) {
    // Convenience function to create a scene with specified camera, screen, lighting, and background color
    return Scene(&camera, &screen, ambient, specular, sp, background);
}

Screen screen(float width = 10.0f, float height = 10.0f) {
    // Convenience function to create a screen with specified width and height
    return Screen(width, height);
}

void sphere(Point center, float radius, BasicMaterial material, Scene& scene) {
    // Convenience function to create a basic sphere shape
    scene.add_shape(std::make_unique<BasicSphere<>>(center, radius, material));
}

void plane(Point point, Vector normal, BasicMaterial material, Scene& scene) {
    // Convenience function to create a basic plane shape
    scene.add_shape(std::make_unique<BasicPlane<>>(point, normal, material));
}
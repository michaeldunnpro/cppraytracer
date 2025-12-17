#include <cassert>
#include <iostream>
#include <memory>

#include "color.hpp"
#include "material.hpp"
#include "materials/basic.hpp"
#include "shape.hpp"
#include "shapes/plane.hpp"
#include "shapes/sphere.hpp"
#include "ui.hpp"
#include "vector.hpp"

void test_scene() {
    Camera camera(Point(0.5f, -1.5f, 0.5f), Vector(0.0f, 1.0f, 0.0f));
    Screen screen(10.0f, 10.0f);
    Scene scene(&camera, &screen, 0.8f, 0.5f, 8., Color::from_rgb(135, 206, 235));
    BasicMaterial red_material(Color::from_rgb(255, 0, 0), 0.4f);
    scene.add_shape(std::make_unique<BasicSphere<>>(Point(0.25f, 0.45f, 0.4f), 0.4f, red_material));
    BasicMaterial green_material(Color::from_rgb(0, 255, 0), 0.2f);
    scene.add_shape(std::make_unique<BasicSphere<>>(Point(1.0f, 1.0f, 0.25f), 0.25f, green_material));
    BasicMaterial blue_material(Color::from_rgb(0, 0, 255), 0.7f);
    scene.add_shape(std::make_unique<BasicSphere<>>(Point(0.8f, 0.3f, 0.15f), 0.15f, blue_material));
    BasicMaterial gray_material(Color::from_rgb(200, 200, 200), 0.5f);
    scene.add_shape(std::make_unique<BasicPlane<>>(Point(0.0f, 0.0f, -0.1f), Vector(0.0f, 0.0f, 1.0f), gray_material));
    scene.add_light<BasicPointLight>(Point(0.0, -0.5, 1.0));
    make_screen(scene);
}

// Return approximate equality of floats
bool approx_eq(float a, float b) {
    return std::abs(a - b) < 1e-4;
}

int main() {
    Vector v1 = Vector(1.0f, 2.0f, 3.0f);
    Vector v2 = Vector(4.0f, 5.0f, 6.0f);
    Vector v3 = v1 + v2;
    assert(v3 == Vector(5.0f, 7.0f, 9.0f));
    Vector v4 = v2 - v1;
    assert(v4 == Vector(3.0f, 3.0f, 3.0f));
    Vector v5 = v1 * 2.0f;
    assert(v5 == Vector(2.0f, 4.0f, 6.0f));
    Vector v6 = 2.0f * v1;
    assert(v6 == Vector(2.0f, 4.0f, 6.0f));
    Vector v7 = v2 / 2.0f;
    assert(v7 == Vector(2.0f, 2.5f, 3.0f));
    Vector v8 = v1 ^ v2;
    assert(v8 == Vector(-3.0f, 6.0f, -3.0f));
    float dot = v1 * v2;
    assert(dot == 32.0f);
    float mag = ~v1;
    assert(approx_eq(mag, 3.74166f));
    Vector v9 = -v1;
    assert(v9 == Vector(-1.0f, -2.0f, -3.0f));
    Vector v10 = !v1;
    mag = ~v10;
    assert(approx_eq(mag, 1.0f));
    // Test mutability of Point operations
    Point p1 = Point(1.0f, 2.0f, 3.0f);
    p1 = p1 + v1;
    assert(p1 == Point(2.0f, 4.0f, 6.0f));
    v1 = Vector(1.0f, 2.0f, 3.0f);
    assert(v1 == Vector(1.0f, 2.0f, 3.0f));
    std::cout << "Vector and Point classes compiled successfully." << std::endl;

    // Use 1.0f in Color::from_rgb() and Color::get_rgb() to disable
    // gamma correction, so that operation results are easier to predict
    Color c1 = Color::from_rgb(100.0f, 150.0f, 200.0f, 1.0f);
    Color c2 = Color::from_rgb(50.0f, 75.0f, 100.0f, 1.0f);

    Color c3 = c1 + c2;
    auto c3_rgb = c3.get_rgb(1.0f);
    assert(approx_eq(c3_rgb[0], 150.0f) && approx_eq(c3_rgb[1], 225.0f)
        && approx_eq(c3_rgb[2], 300.0f));

    Color c4 = c1 - c2;
    auto c4_rgb = c4.get_rgb(1.0f);
    assert(approx_eq(c4_rgb[0], 50.0f) && approx_eq(c4_rgb[1], 75.0f)
        && approx_eq(c4_rgb[2], 100.0f));

    Color c5 = c1 * 2.0f;
    auto c5_rgb = c5.get_rgb(1.0f);
    assert(approx_eq(c5_rgb[0], 200.0f) && approx_eq(c5_rgb[1], 300.0f)
        && approx_eq(c5_rgb[2], 400.0f));

    Color c6 = 0.5f * c2;
    auto c6_rgb = c6.get_rgb(1.0f);
    assert(approx_eq(c6_rgb[0], 25.0f) && approx_eq(c6_rgb[1], 37.5f)
        && approx_eq(c6_rgb[2], 50.0f));
    
    Color filter = Color::raw(0.0f, 0.5f, 1.0f);
    Color c7 = filter * c1;
    auto c7_rgb = c7.get_rgb(1.0f);
    assert(approx_eq(c7_rgb[0], 0.0f) && approx_eq(c7_rgb[1], 75.0f)
        && approx_eq(c7_rgb[2], 200.0f));

    c3.clamp();
    c3_rgb = c3.get_rgb(1.0f);
    assert(approx_eq(c3_rgb[0], 150.0f) && approx_eq(c3_rgb[1], 225.0f)
        && approx_eq(c3_rgb[2], 255.0f)); // b should be clamped to 255

    std::cout << "Color class compiled successfully." << std::endl;
    test_scene();
    return 0;
}
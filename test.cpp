#include <cassert>
#include <iostream>
#include <memory>

#include "color.hpp"
#include "material.hpp"
#include "shape.hpp"
#include "vector.hpp"

void test_scene() {
    Camera camera(Point(0.5f, -1.5f, 0.5f), Vector(0.0f, 1.f, 0.0f));
    Screen screen(10.f, 10.f);
    Scene scene(&camera, &screen, 0.8f, 0.5f, 8., Color(135, 206, 235));
    BasicMaterial red_material(Color(255, 0, 0), 0.4f);
    scene.add_shape(std::make_unique<BasicSphere<>>(Point(0.25f, 0.45f, 0.4f), 0.4f, red_material));
    BasicMaterial green_material(Color(0, 255, 0), 0.2f);
    scene.add_shape(std::make_unique<BasicSphere<>>(Point(1.f, 1.f, 0.25f), 0.25f, green_material));
    BasicMaterial blue_material(Color(0, 0, 255), 0.7f);
    scene.add_shape(std::make_unique<BasicSphere<>>(Point(0.8f, 0.3f, 0.15f), 0.15f, blue_material));
    BasicMaterial gray_material(Color(200, 200, 200), 0.5f);
    scene.add_shape(std::make_unique<BasicPlane<>>(Point(0.f, 0.f, -0.1f), Vector(0.f, 0.f, 1.f), gray_material));
    scene.add_point_light(Point(0.0, -0.5, 1.0));
    scene.make_screen();
    handle_input(scene);
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
    assert(std::abs(mag - 3.74166f) < 0.0001f);
    Vector v9 = -v1;
    assert(v9 == Vector(-1.0f, -2.0f, -3.0f));
    Vector v10 = !v1;
    mag = ~v10;
    assert(std::abs(mag - 1.0f) < 0.0001f);
    // Test mutability of Point operations
    Point p1 = Point(1.0f, 2.0f, 3.0f);
    p1 = p1 + v1;
    assert(p1 == Point(2.0f, 4.0f, 6.0f));
    v1 = Vector(1.0f, 2.0f, 3.0f);
    assert(v1 == Vector(1.0f, 2.0f, 3.0f));
    std::cout << "Vector and Point classes compiled successfully." << std::endl;
    Color c1(100.0f, 150.0f, 200.0f);
    Color c2(50.0f, 75.0f, 100.0f);
    Color c3 = c1 + c2;
    assert(c3.getRGB()[0] == 150.0f && c3.getRGB()[1] == 225.0f
        && c3.getRGB()[2] == 300.0f);
    Color c4 = c1 - c2;
    assert(c4.getRGB()[0] == 50.0f && c4.getRGB()[1] == 75.0f
        && c4.getRGB()[2] == 100.0f);
    Color c5 = c1 * 2.0f;
    assert(c5.getRGB()[0] == 200.0f && c5.getRGB()[1] == 300.0f
        && c5.getRGB()[2] == 400.0f);
    Color c6 = 0.5f * c2;
    assert(c6.getRGB()[0] == 25.0f && c6.getRGB()[1] == 37.5f
        && c6.getRGB()[2] == 50.0f);
    c3.clamp();
    assert(c3.getRGB()[0] == 150.0f && c3.getRGB()[1] == 225.0f
        && c3.getRGB()[2] == 255.0f); // b should be clamped to 255
    std::cout << "Color class compiled successfully." << std::endl;
    
    test_scene();
    return 0;
}
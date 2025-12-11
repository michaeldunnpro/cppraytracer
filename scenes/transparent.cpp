#include "../src/scene_constructor.hpp"

// TO RUN: make scene SCENE=scenes/pattern.cpp
// Cam at (0.5, -1.5, 0.5) looking towards (0,1,0)
// Screen 10x10 units with distance 10
// Spheres at (0.25, 0.45, -0.1) radius 0.15 red
//            (1, 1, 0.25) radius 0.25 green
//            (0.8, 0.3, 0.15) radius 0.15 transparent
// Plane at z = -0.3 with red/blue grid pattern
// Plane at z = 0 transparent
// Point light at (0, -0.5, 1)

int main() {
    // Create scene components
    auto camera = cam(Point(0.5f, -1.5f, 0.5f), Vector(0.0f, 1.0f, 0.0f));
    auto scr = screen(10.0f, 10.0f);
    auto scn = scene(camera, scr, 0.8f, 0.5f, 8.0f, Color(135, 206, 235));

    // Define materials
    auto red = mat(Color(255, 0, 0), 0.4f);
    auto green = mat(Color(0, 255, 0), 0.2f);
    TransparentMaterial glass(1.6);
    TransparentMaterial water(1.33);

    // Add simple shapes to the scene
    sphere(Point(0.25f, 0.45f, -0.1f), 0.15f, red, scn);
    sphere(Point(1.0f, 1.0f, 0.25f), 0.25f, green, scn);

    // Add a plane with color grid pattern
    auto pattern = [](float a, float b) {
        int t = std::abs((int)std::floor(10 * a) + (int)std::floor(10 * b)) % 2;
        return std::make_unique<BasicMaterial>(t * Color(255, 0, 0) + (1 - t) * Color(0, 0, 255), 0.5f);
    };
    scn.add_shape(std::make_unique<ParametricPlane<decltype(pattern)>>(
        Point(0.0f, 0.0f, -0.3f),
        Vector(1.0f, 0.0f, 0.0f),
        Vector(0.0f, 1.0f, 0.0f),
        pattern));

    // Add a transparent sphere
    scn.add_shape(std::make_unique<BasicSphere<TransparentMaterial>>(Point(0.8f, 0.3f, 0.15f), 0.15f, glass));

    // Add a transparent plane above the existing plane
    scn.add_shape(std::make_unique<BasicPlane<TransparentMaterial>>(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), water));

    // Add point light to the scene
    scn.add_point_light(Point(0.0, -0.5, 1.0));

    handle_input(scn);
    return 0;
}

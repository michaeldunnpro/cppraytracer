#include "../src/scene_constructor.hpp"

// TO RUN: make scene SCENE=scenes/pattern.cpp
// Cam at (0, -1.5, 0.5) looking towards (0, 1, -0.2) (forward but lower)
// Screen 10x10 units with distance 10
// Spheres at (-0.25, 0, -0.15) radius 0.25 yellow
//            (0.3, 0.3, 0.3) radius 0.3 transparent
// Plane at z = -0.5 with red/blue grid pattern
// Plane at z = 0 transparent
// Point light at (0.5, -0.5, 1)

int main() {
    // Create scene components
    auto camera = cam(Point(0.0f, -1.5f, 0.5f), Vector(0.0f, 1.0f, -0.2f));
    auto scr = screen(10.0f, 10.0f);
    auto scn = scene(camera, scr, 0.2f, 0.5f, 8.0f, rgb(135, 206, 235));

    auto yellow = mat(rgb(255, 255, 0), 0.2f);
    TransparentMaterial glass(1.6);
    TransparentMaterial water(1.33);

    // Add a plane with color grid pattern
    auto pattern = [](float a, float b) {
        int t = std::abs((int)std::floor(10 * a) + (int)std::floor(10 * b)) % 2;
        return std::make_unique<BasicMaterial>(t * rgb(255, 0, 0) + (1 - t) * rgb(0, 0, 255), 0.2f);
    };
    scn.add_shape(std::make_unique<ParametricPlane<decltype(pattern)>>(
        Point(0.0f, 0.0f, -0.5f),
        Vector(1.0f, 0.0f, 0.0f),
        Vector(0.0f, 1.0f, 0.0f),
        pattern));

    // Add a transparent plane above the existing one to represent water
    plane(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), water, scn);

    // Add a sphere partially in water
    sphere(Point(-0.25f, 0.0f, -0.15f), 0.25f, yellow, scn);

    // Add a transparent sphere just above water
    sphere(Point(0.3f, 0.3f, 0.3f), 0.3f, glass, scn);

    // Add point light to the scene
    scn.add_light<BasicPointLight>(Point(0.5, -0.5, 1.0));

    handle_input(scn);
    return 0;
}

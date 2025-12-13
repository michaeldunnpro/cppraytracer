#include "../src/scene_constructor.hpp"

// TO RUN: make scene SCENE=scenes/pattern.cpp
// Cam at (0.5, -1.5, 0.5) looking towards (0,1,0)
// Screen 10x10 units with distance 10
// Spheres at (0.25, 0.45, 0.4) radius 0.1 copper metal
//            (1, 1, 0.25) radius 0.25 iron metal rough
//            (0.8, 0.3, 0.15) radius 0.15 brown non-metal
// Plane at z = -0.1 gray non-metal rough
// Point light at (0, -0.5, 1) inverse-square

int main() {
    // Create scene components
    auto camera = cam(Point(0.5f, -1.5f, 0.5f), Vector(0.0f, 1.0f, 0.0f));
    auto scr = screen(10.0f, 10.0f);
    auto scn = scene(camera, scr, 0.1f, 0.5f, 8.0f, Color::from_rgb(135, 206, 235));

    // Define materials
    PBRMaterial copper(Color::from_rgb(0.95, 0.64, 0.54, 1.0f) * 255.0, 0.2f, 1.0f);
    // rough metal needs more samples for specular reflection
    PBRMaterial iron(Color::from_rgb(0.56, 0.57, 0.58, 1.0f) * 255.0, 0.5f, 1.0f, 0.5f, 256);
    PBRMaterial brown(Color::from_rgb(165, 42, 42), 0.2f, 0.0f);
    PBRMaterial gray(Color::from_rgb(200, 200, 200), 0.5f, 0.0f);

    // Add simple shapes to the scene
    sphere(Point(0.25f, 0.45f, 0.4f), 0.1f, copper, scn);
    sphere(Point(1.0f, 1.0f, 0.25f), 0.25f, iron, scn);
    sphere(Point(0.8f, 0.3f, 0.15f), 0.15f, brown, scn);
    plane(Point(0.0f, 0.0f, -0.1f), Vector(0.0f, 0.0f, 1.0f), gray, scn);

    // Add point light to the scene
    scn.add_light<InverseSquarePointLight>(Point(0.0, -0.5, 1.0), Color::from_rgb(255, 255, 255), 5.0f);

    handle_input(scn);
    return 0;
}

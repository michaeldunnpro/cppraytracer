// Example scene definition

// This must be included at top of scene file
#include "../src/scene_constructor.hpp"
//


// TO RUN: make scene SCENE=scenes/example_scene.cpp
// Cam at (0.5, -1.5, 0.5) looking towards (0,1,0)
// Screen 10x10 units
// Spheres at (0.25, 0.45, 0.4) radius 0.1 red
//            (1, 1, 0.25) radius 0.25 green
//            (0.8, 0.3, 0.15) radius 0.15 blue
// Plane at z = -0.1 gray
// Point light at (0, -0.5, 1)

int main() {
    // Create scene components (Must do this first)
    auto camera = cam(Point(0.5f, -1.5f, 0.5f), Vector(0.0f, 1.0f, 0.0f));
    auto scr = screen(10.0f, 10.0f);
    auto scn = scene(camera, scr, 0.8f, 0.5f, 8.0f, Color(135, 206, 235));

    // Define materials
    auto red = mat(Color(255, 0, 0), 0.4f);
    auto green = mat(Color(0, 255, 0), 0.2f);
    auto blue = mat(Color(0, 0, 255), 0.7f);
    auto gray = mat(Color(200, 200, 200), 0.5f);

    // Add shapes to the scene
    sphere(Point(0.25f, 0.45f, 0.4f), 0.1f, red, scn);
    sphere(Point(1.0f, 1.0f, 0.25f), 0.25f, green, scn);
    sphere(Point(0.8f, 0.3f, 0.15f), 0.15f, blue, scn);
    plane(Point(0.0f, 0.0f, -0.1f), Vector(0.0f, 0.0f, 1.0f), gray, scn);
    
    // Add point light to the scene
    scn.add_point_light(Point(0.0, -0.5, 1.0));

    // Handle user input and render the scene
    handle_input(scn);
    return 0;
}

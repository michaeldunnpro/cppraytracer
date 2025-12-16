// Example scene definition

// This must be included at top of scene file
#include "../src/scene_constructor.hpp"
//

// TO RUN: make scene SCENE=scenes/example_scene2.cpp

int main() {
    // Create scene components (Must do this first)
    auto camera = cam(Point(0.0f, -1.5f, 0.6f), Vector(0.0f, 1.0f, 0.0f));
    auto scr = screen(10.0f, 10.0f);
    auto scn = scene(camera, scr, 0.6f, 0.5f, 8.0f, rgb(141, 157, 184));

    // Define materials
    auto teal = mat(rgb(96, 209, 168), 0.0f);
    auto darkBlue = mat(rgb(47, 30, 94), 0.5f);
    auto darkPink = mat(rgb(138, 51, 94), 0.2f);
    auto purple = mat(rgb(67, 52, 89), 0.3f);
    auto darkGrey = mat(rgb(61, 64, 62), 0.3f);

    // Add shapes to the scene
    sphere(Point(0.0f, 0.0f, 0.2f), 0.2f, teal, scn);
    sphere(Point(0.0f, 0.2f, 0.6f), 0.15f, darkBlue, scn);
    sphere(Point(-0.4f, 0.0f, 0.6f), 0.2f, darkPink, scn);
    sphere(Point(0.4f, 0.0f, 0.6f), 0.2f, darkPink, scn);
    sphere(Point(0.0f, 0.0f, 1.0f), 0.2f, teal, scn);

    plane(Point(0.0f, 0.0f, -0.1f), Vector(0.0f, 0.0f, 1.0f), darkGrey, scn);

    // Add point light to the scene
    Color candle = rgb(255, 147, 41);
    scn.add_light<BasicPointLight>(Point(0.0, -0.5, 1.0));
    scn.add_light<InverseSquarePointLight>(Point(2.0, -0.5, 1.0), candle, 3.0);
    scn.add_light<InverseSquarePointLight>(Point(-2.0, -0.5, 1.0), candle, 3.0);

    handle_input(scn);
    return 0;
}

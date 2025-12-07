#pragma once

#include "scene.hpp"

/**
 * @brief Render the scene and write the output into `image.ppm`.
 */
void make_screen(Scene const& scene, int width = 480, int height = 480);

// Render the scene and output to terminal
void make_screen_terminal(Scene const& scene);

// Implementation of input handling for camera movement
void handle_input(Scene const& scene);
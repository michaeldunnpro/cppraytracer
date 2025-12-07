#include <fstream>
#include <iostream>
#include <sys/ioctl.h> // For terminal size detection
#include <unistd.h>

#include "ui.hpp"

// Note compatibility requires a Unix-like system for terminal size detection

void make_screen(Scene const& scene, int width, int height) {
    std::ofstream Image("image.ppm");
    Image << "P3" << std::endl; // P3 format .ppm file
    // tells the file the length and width of the image
    Image << width << " " << height << std::endl;
    Image << "255" << std::endl; // RGB values rated out of 255

    std::vector<Color> data = scene.render(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Color color = data[i * width + j];
            std::unique_ptr<float[]> rgb = color.getRGB();
            Image << (int)rgb[0] << " " << (int)rgb[1] << " " << (int)rgb[2] << std::endl;
        }
    }
    Image.close();
}

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int get_terminal_height() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

void make_screen_terminal(Scene const& scene) {
    // Disable line wrap to avoid automatic wrapping in some terminals
    std::cout << "\033[?7l";

    auto rgb_to_256 = [](int r, int g, int b) { // Helper function to convert RGB to 256-color index
        int rr = r / 51;
        int gg = g / 51;
        int bb = b / 51;
        return 16 + 36 * rr + 6 * gg + bb; // xterm 6x6x6 color cube
    };

    // Account for two-character cells ("  ") and leave a small margin
    int raw_width = std::max(1, get_terminal_width() - 2);
    int raw_height = std::max(1, get_terminal_height() - 2); // leave a line for prompts
    int term_width = std::max(1, raw_width / 2); // two spaces per pixel
    int term_height = raw_height;

    float width = scene.get_screen()->get_width();
    float length = scene.get_screen()->get_length();
    float aspect_ratio = width / length;
    term_width = std::max(1, (int)(term_height * aspect_ratio)); // adjust width

    // Clear screen and move cursor home
    std::cout << "\033[2J\033[H";

    std::vector<Color> data = scene.render(term_width, term_height);

    for (int i = 0; i < term_height; i++) {
        // Add padding to center the image
        int padding = (raw_width - (term_width) * 2) / 2;
        for (int p = 0; p < padding; p++) {
            std::cout << " ";
        }
        // Render each pixel in the row
        for (int j = 0; j < term_width; j++) {
            Color color = data[i * term_width + j];
            std::unique_ptr<float[]> rgb = color.getRGB();
            int idx = rgb_to_256((int)rgb[0], (int)rgb[1], (int)rgb[2]);
            std::cout << "\033[48;5;" << idx << "m  \033[0m"; // 256-color background
        }
        std::cout << '\n';
    }

    // Re-enable line wrap
    std::cout << "\033[?7h" << std::flush; // Ensure all output is printed
}

void handle_input(Scene const& scene) {
    Camera& camera = *scene.get_camera();
    constexpr float kPi = 3.14159265358979323846f; // C++17-safe pi constant
    while (true) {
        std::cout << "Enter command (w/a/s/d to move, r/f to go up/down, i/j/k/l to look, q to quit): ";
        char command;
        std::cin >> command;
        switch (command) {
            case 'w': {
                camera.set_position(camera.get_position() + 0.1 * camera.get_orientation());
                break;
            }
            case 's': {
                camera.set_position(camera.get_position() - 0.1 * camera.get_orientation());
                break;
            }
            case 'a': {
                Vector left = !(camera.get_orientation().rotate(kPi / 2.0f, 0, 0));
                camera.set_position(camera.get_position() + 0.1 * left);
                break;
            }
            case 'd': {
                Vector right = !(camera.get_orientation().rotate(-kPi / 2.0f, 0, 0));
                camera.set_position(camera.get_position() + 0.1 * right);
                break;
            }
            case 'r': {
                Vector up = !(camera.get_orientation().rotate(0, 0, kPi / 2.0f));
                camera.set_position(camera.get_position() + 0.1 * up);
                break;
            }
            case 'f': {
                Vector down = !(camera.get_orientation().rotate(0, 0, -kPi / 2.0f));
                camera.set_position(camera.get_position() + 0.1 * down);
                break;
            }
            case 'i': {
                Vector new_orientation = !camera.get_orientation().rotate(0, 0, kPi / 10.0f);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'k': {
                Vector new_orientation = !camera.get_orientation().rotate(0, 0, -kPi / 10.0f);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'j': {
                Vector new_orientation = !camera.get_orientation().rotate(kPi / 10.0f, 0, 0);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'l': {
                Vector new_orientation = !camera.get_orientation().rotate(-kPi / 10.0f, 0, 0);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'q': {
                make_screen(scene); // Save final image
                return;
            }
            default: {
                std::cout << "Unknown command\n";
                break;
            }
        }
        make_screen_terminal(scene); // Re-render the scene after each command
    }
}
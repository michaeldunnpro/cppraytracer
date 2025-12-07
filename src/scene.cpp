#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/ioctl.h> // For terminal size detection
#include <unistd.h>

#include "scene.hpp"

// Note compatibility requires a Unix-like system for terminal size detection

Camera::Camera(Point pos, Vector ori)
    : position(pos)
    , orientation(ori) {
}

Point Camera::get_position() const {
    return this->position;
}

Vector Camera::get_orientation() const {
    return this->orientation;
}

void Camera::set_position(Point pos) {
    this->position = pos;
}

void Camera::set_orientation(Vector ori) {
    this->orientation = ori;
}

Screen::Screen(float wid, float len, int pix_wid, int pix_len, int d_c)
    : width(wid)
    , length(len)
    , pixel_width(pix_wid)
    , pixel_length(pix_len)
    , dst_cam(d_c) {
}

float Screen::get_width() const {
    return this->width;
}

float Screen::get_length() const {
    return this->length;
}

int Screen::get_pixel_width() const {
    return this->pixel_width;
}

int Screen::get_pixel_length() const {
    return this->pixel_length;
}

int Screen::get_dst_cam() const {
    return this->dst_cam;
}

void Screen::set_dst_cam(int d_c) {
    this->dst_cam = d_c;
}

Point Screen::get_pixel(int x, int y, Camera* cam) const {
    // point camera is at moved by dst_cam in the direction of orientation
    Point origin = cam->get_position() + (!(cam->get_orientation()) * this->get_dst_cam());

    // Offsets in horizontal and verticle directions in space coordinates
    float x_offset = ((float)x / this->get_pixel_width() - 0.5f) * this->get_width();
    // Flip Y so increasing screen y points downward in image but upward in world
    float y_offset = (0.5f - (float)y / this->get_pixel_length()) * this->get_length();

    Vector x_vector = !(cam->get_orientation()) ^ Vector(0, 0, 1);
    Vector y_vector = x_vector ^ cam->get_orientation();

    Point pixel = origin + (x_offset * x_vector) + (y_offset * y_vector); // move pixel by x and y
    return pixel;
}

Scene::Scene(Camera* cam, Screen* scr, float ambient, float specular, float sp, Color background)
    : camera(cam)
    , screen(scr)
    , ambient(ambient)
    , specular(specular)
    , sp(sp)
    , background(background) {
}

Camera* Scene::get_camera() const {
    return this->camera;
}

Screen* Scene::get_screen() const {
    return this->screen;
}

float Scene::get_ambient() const {
    return this->ambient;
}

float Scene::get_specular() const {
    return this->specular;
}

float Scene::get_sp() const {
    return this->sp;
}

Color Scene::get_background() const {
    return this->background;
}

void Scene::add_shape(std::unique_ptr<Shape>&& shape) {
    this->shapes.push_back(std::move(shape));
}

void Scene::add_point_light(Point point) {
    this->point_lights.push_back(point);
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

void Scene::make_screen_terminal() { // for each pixel on the screen, trace a ray from camera to pixel. Output to terminal
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

    int pixel_width = this->screen->get_pixel_width();
    int aspect_ratio = pixel_width / this->screen->get_pixel_length();
    term_width = std::max(1, term_height * aspect_ratio); // adjust width
    int pixel_length = this->screen->get_pixel_length();
    int step_x = std::max(1, pixel_width / term_width);
    int step_y = std::max(1, pixel_length / term_height);

    // Clear screen and move cursor home
    std::cout << "\033[2J\033[H";

    for (int i = 0; i < this->screen->get_pixel_length(); i += step_y) {
        // Add padding to center the image
        int padding = (raw_width - (this->screen->get_pixel_width() / step_x) * 2) / 2;
        for (int p = 0; p < padding; p++) {
            std::cout << " ";
        }
        // Render each pixel in the row
        for (int j = 0; j < this->screen->get_pixel_width(); j += step_x) {
            Point destination = this->get_screen()->get_pixel(j, i, this->camera);
            Vector direction = destination - this->get_camera()->get_position();
            Color color = trace(Ray(this->camera->get_position(), direction), this->recursion_depth);
            color.clamp();
            std::unique_ptr<float[]> rgb = color.getRGB();
            int idx = rgb_to_256((int)rgb[0], (int)rgb[1], (int)rgb[2]);
            std::cout << "\033[48;5;" << idx << "m  \033[0m"; // 256-color background
        }
        std::cout << '\n';
    }
    // Re-enable line wrap
    std::cout << "\033[?7h" << std::flush; // Ensure all output is printed
}

void Scene::make_screen() { // for each pixel on the screen, trace a ray from the camera to that pixel
    std::ofstream Image("image.ppm");
    Image << "P3" << std::endl; // P3 format .ppm file
    // tells the file the length and width of the image
    Image << this->screen->get_pixel_width() << " " << this->screen->get_pixel_length() << std::endl;
    Image << "255" << std::endl; // RGB values rated out of 255
    for (int i = 0; i < this->screen->get_pixel_length(); i++) {
        for (int j = 0; j < this->screen->get_pixel_width(); j++) {
            Point destination = this->get_screen()->get_pixel(j, i, this->camera);
            Vector direction = destination - this->get_camera()->get_position();
            Color color = trace(Ray(this->camera->get_position(), direction), this->recursion_depth);
            color.clamp();
            std::unique_ptr<float[]> rgb = color.getRGB();
            Image << (int)rgb[0] << " " << (int)rgb[1] << " " << (int)rgb[2] << std::endl;
        }
    }
    Image.close();
}

std::optional<std::pair<float, std::reference_wrapper<Shape const>>> Scene::intersect_first_all(Ray const& ray) const {
    // Track the closest intersection the ray meets by far
    std::optional<std::pair<float, std::reference_wrapper<Shape const>>> min_intersection {};

    for (auto&& shape : this->shapes) {
        std::optional<float> t = shape->intersect_first(ray);
        // Update `min_intersection` when there is a new intersection, and
        // either there is currently no other intersections
        // or the new intersection is smaller than the current one
        if (t && (!min_intersection || t.value() < min_intersection.value().first)) {
            min_intersection = std::make_pair(t.value(), std::cref(*shape));
        }
    }

    return min_intersection;
}

std::vector<Point> Scene::get_visible_point_lights(Point const& point) const {
    std::vector<Point> visible_point_lights;

    // Iterate over the point lights in the scene
    for (auto&& point_light : this->point_lights) {
        // Form the ray pointing from `point` to the light source
        Ray ray(point, point_light - point);
        std::optional<std::pair<float, std::reference_wrapper<Shape const>>> min_intersection = this->intersect_first_all(ray);

        // Check if the ray intersects any shape
        if (min_intersection) {
            auto [t, shape] = min_intersection.value();
            // Check if the ray intersects any shape before meeting the light source
            if (t <= 1) {
                // There is some obstacle between `point` and the light source
                break;
            }
        }

        // There is no obstacle between `point` and the light source
        visible_point_lights.push_back(point_light);
    }

    return visible_point_lights;
}

Color Scene::trace(Ray const& ray, int recursion_depth) const {
    // Compute the first intersection (if any)
    std::optional<std::pair<float, std::reference_wrapper<Shape const>>> min_intersection = this->intersect_first_all(ray);

    if (min_intersection) {
        auto [t, shape] = min_intersection.value();
        Point point = ray.at(t);

        // Get the material at the intersection point
        std::unique_ptr<Material> material = shape.get().material_at(point);

        // Get the normal vector at the intersection point and make sure it point out
        // i.e., in opposite direction with the incoming ray
        Vector normal = shape.get().normal_at(point);
        if (normal * ray.direction > 0) {
            normal = -normal;
        }

        return material->get_color(ray.direction, point, normal, this, recursion_depth);
    }

    return this->background;
}




void handle_input(Scene& scene) {
    Camera &camera = *scene.get_camera();
    constexpr float kPi = 3.14159265358979323846f; // C++17-safe pi constant
    while (true) {
        std::cout << "Enter command (w/a/s/d to move, r/f to go up/down, i/j/k/l to look, q to quit): ";
        char command;
        std::cin >> command;
        switch (command) {
            case 'w': {
                camera.set_position(camera.get_position() + 0.1* camera.get_orientation());
                break;
            }
            case 's': {
                camera.set_position(camera.get_position() - 0.1* camera.get_orientation());
                break;
            }
            case 'a' : {
                Vector left = !(camera.get_orientation().Rotate(kPi / 2.0f, 0, 0));
                camera.set_position(camera.get_position() + 0.1* left);
                break;
            }
            case 'd' : {
                Vector right = !(camera.get_orientation().Rotate(-kPi / 2.0f, 0, 0));
                camera.set_position(camera.get_position() + 0.1* right);
                break;
            }
            case 'r' : {
                Vector up = !(camera.get_orientation().Rotate(0, 0, kPi / 2.0f));
                camera.set_position(camera.get_position() + 0.1* up);
                break;
            }
            case 'f' : {
                Vector down = !(camera.get_orientation().Rotate(0, 0, -kPi / 2.0f));
                camera.set_position(camera.get_position() + 0.1* down);
                break;
            }
            case 'i' : {
                Vector new_orientation = !camera.get_orientation().Rotate(0, 0, kPi / 10.0f);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'k' : {
                Vector new_orientation = !camera.get_orientation().Rotate(0, 0, -kPi / 10.0f);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'j' : {
                Vector new_orientation = !camera.get_orientation().Rotate(kPi / 10.0f, 0, 0);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'l' : {
                Vector new_orientation = !camera.get_orientation().Rotate(-kPi / 10.0f, 0, 0);
                camera.set_orientation(new_orientation);
                break;
            }
            case 'q' : {
                scene.make_screen(); // Save final image
                return;
            }
            default: {
                std::cout << "Unknown command\n";
                break;
            }
        }
        scene.make_screen_terminal(); // Re-render the scene after each command
    }
}
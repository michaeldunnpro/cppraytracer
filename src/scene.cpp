#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono> // for measuring rendering time

#include "scene.hpp"

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

Screen::Screen(float wid, float len, int d_c)
    : width(wid)
    , length(len)
    , dst_cam(d_c) {
}

float Screen::get_width() const {
    return this->width;
}

float Screen::get_length() const {
    return this->length;
}

int Screen::get_dst_cam() const {
    return this->dst_cam;
}

void Screen::set_dst_cam(int d_c) {
    this->dst_cam = d_c;
}

Point Screen::get_pixel(float x, float y, Camera* cam) const {
    // point camera is at moved by dst_cam in the direction of orientation
    Point origin = cam->get_position() + (!(cam->get_orientation()) * this->get_dst_cam());

    // Offsets in horizontal and verticle directions in space coordinates
    float x_offset = (x - 0.5f) * this->get_width();
    // Flip Y so increasing screen y points downward in image but upward in world
    float y_offset = (0.5f - y) * this->get_length();

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

void Scene::add_light(std::unique_ptr<Light>&& light) {
    this->lights.push_back(std::move(light));
}

std::vector<Color> Scene::render(int width, int height) const {
    std::vector<Color> output;
    // The size of the output vector is known, so reserve the space needed in advance
    output.reserve(width * height);
    // Here's the hot loop of the ray tracer
    auto start_time = std::chrono::high_resolution_clock::now(); // Start measuring time
    #pragma omp parallel for schedule(dynamic) // Parallelize the outer loop with OpenMP
    for (int i = 0; i < height; i++) {
        // adjust by 0.5 so that the ray points to the center of the pixel
        // instead of the top-left corner
        float screen_y = ((float)i + 0.5f) / height;
        for (int j = 0; j < width; j++) {
            float screen_x = ((float)j + 0.5f) / width;
            Point destination = this->screen->get_pixel(screen_x, screen_y, this->camera); // TODO: check order
            Vector direction = destination - this->camera->get_position();
            Color color = trace(Ray(this->camera->get_position(), direction), this->recursion_depth);
            color.clamp();
            output[i * width + j] = color; // Thread safely write to different indices of the output vector without needing synchronization
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now(); // End measuring time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Rendering completed in " << duration << " milliseconds." << std::endl;
    return output;
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

std::vector<std::reference_wrapper<Light const>> Scene::get_visible_point_lights(Point const& point) const {
    std::vector<std::reference_wrapper<Light const>> visible_lights {};
    for (auto&& light : this->lights) {
        if (light->is_visible(point, *this)) {
            visible_lights.push_back(std::cref(*light));
        }
    }

    return visible_lights;
}

Color Scene::trace(Ray const& ray, int recursion_depth) const {
    // Compute the first intersection (if any)
    std::optional<std::pair<float, std::reference_wrapper<Shape const>>> min_intersection = this->intersect_first_all(ray);

    if (min_intersection) {
        auto [t, shape] = min_intersection.value();
        Point point = ray.at(t);

        // Get the material at the intersection point
        std::unique_ptr<Material> material = shape.get().material_at(point);

        // Get the normal vector
        Vector normal = shape.get().normal_at(point);

        return material->get_color(ray.direction, point, normal, this, recursion_depth);
    }

    return this->background;
}

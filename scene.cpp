#include <fstream>
#include <iostream>

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
    float y_offset = ((float)y / this->get_pixel_length() - 0.5f) * this->get_length();

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

#include "scene.hpp"

std::optional<std::pair<double, Shape const&>> Scene::intersect_first_all(Ray const& ray) const {
    std::optional<std::pair<float, Shape const&>> min_intersection {};
    for (auto&& shape : this->shapes) {
        std::optional<float> t = shape.intersect_first(ray);
        if (!min_intersection || (t && t.value() < min_intersection.value().first)) {
            min_intersection = std::make_pair(t.value(), shape);
        }
    }

    return min_intersection;
}

std::vector<Point> const& Scene::get_visible_point_lights(Point const& point) const {
    std::vector<Point> visible_point_lights {};

    for (auto&& point_light : this->point_lights) {
        Ray ray(point, point_light - point);
        std::optional<std::pair<float, Shape const&>> min_intersection = this->intersect_first_all(ray);

        if (min_intersection) {
            auto [t, shape] = min_intersection.value();
            if (t <= 1) {
                // There is some obstacle between `point` and the light source
                break;
            }
        }

        visible_point_lights.push_back(point_light);
    }

    return visible_point_lights;
}

Color Scene::trace(Ray const& ray, int recursion_depth) const {
    std::optional<std::pair<float, Shape const&>> min_intersection = this->intersect_first_all(ray);

    if (min_intersection) {
        auto [t, shape] = min_intersection.value();
        Point point = ray.at(t);
        std::unique_ptr<Material> material = shape.material_at(point);
        Vector normal = shape.normal_at(point);
        return material->get_color(ray.direction, point, normal, this, recursion_depth);
    }

    // TODO: background color
    return Color::white();
}

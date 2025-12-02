#include "scene.hpp"

std::optional<std::pair<double, Shape const&>> Scene::intersect_first_all(Ray const& ray) const {
    // Track the closest intersection the ray meets by far
    std::optional<std::pair<float, Shape const&>> min_intersection {};

    for (auto&& shape : this->shapes) {
        std::optional<float> t = shape.intersect_first(ray);
        // Update `min_intersection` when either
        // there is currently no other intersections, or
        // there is a new intersection that is smaller than the current one
        if (!min_intersection || (t && t.value() < min_intersection.value().first)) {
            min_intersection = std::make_pair(t.value(), shape);
        }
    }

    return min_intersection;
}

std::vector<Point> const& Scene::get_visible_point_lights(Point const& point) const {
    std::vector<Point> visible_point_lights {};

    // Iterate over the point lights in the scene
    for (auto&& point_light : this->point_lights) {
        // Form the ray pointing from `point` to the light source
        Ray ray(point, point_light - point);
        std::optional<std::pair<float, Shape const&>> min_intersection = this->intersect_first_all(ray);

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
    std::optional<std::pair<float, Shape const&>> min_intersection = this->intersect_first_all(ray);

    if (min_intersection) {
        auto [t, shape] = min_intersection.value();
        Point point = ray.at(t);

        // Get the material at the intersection point
        std::unique_ptr<Material> material = shape.material_at(point);

        // Get the normal vector at the intersection point and make sure it point out
        // i.e., in opposite direction with the incoming ray
        Vector normal = shape.normal_at(point);
        if (normal * ray.direction > 0) {
            normal = -normal;
        }

        return material->get_color(ray.direction, point, normal, this, recursion_depth);
    }

    // TODO: background color
    return Color::white();
}

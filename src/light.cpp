#include "light.hpp"

PointLight::PointLight(Point position)
    : position(position) {
}

Vector PointLight::get_direction(Point point) const {
    return this->position - point;
}

bool PointLight::is_visible(Point point, Scene const& scene) const {
    auto intersection_info = scene.intersect_first_all(Ray(point, this->position - point));
    if (intersection_info) {
        return intersection_info.value().first > 1;
    }
    return true;
}

BasicPointLight::BasicPointLight(Point position, Color color)
    : PointLight(position)
    , color(color) {
}

Color BasicPointLight::get_intensity(Point) const {
    return this->color;
}

InverseSquarePointLight::InverseSquarePointLight(Point position, Color color, float intensity)
    : PointLight(position)
    , color(color * intensity) {
}

Color InverseSquarePointLight::get_intensity(Point point) const {
    Vector diff = this->position - point;
    return (1 / (diff * diff)) * this->color;
}

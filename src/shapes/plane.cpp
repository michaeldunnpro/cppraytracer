#include "plane.hpp"

Plane::Plane(Point point, Vector normal)
    : point(point)
    , normal(!normal) { }

std::optional<float> Plane::intersect_first(Ray const& ray) const {
    float div = ray.direction * this->normal;
    if (std::abs(div) < 1e-6) {
        return {};
    }
    // Use (point - origin) to keep the plane fixed in world space; sign matters
    float t = ((this->point - ray.origin) * this->normal) / div;
    return t > 0 ? std::optional<float>(t) : std::nullopt;
}

Vector Plane::normal_at(Point const&) const {
    return normal;
}